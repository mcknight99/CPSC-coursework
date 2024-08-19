#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h> // for not having to have the raw html in the main
#include "servointerface.h"
#include <ESPmDNS.h>  // Include the mDNS library to maintain a local hostname making it accessible via domain name rather than IP address

const char *ssid = "easywifiname";
const char *password = "easywifipassword";
const String hostname = "lightcontrol"; // The hostname to use for mDNS, will be accessible via lightcontrol.local on the network above

// Create a web server on port 80
WebServer server(80);

// Servo to test while having no test LEDs
ServoInterface ms24;

// Define GPIO pins
const int redPin = 17;
const int yellowPin = 18;
const int greenPin = 14;
const int amberPin = 12;
const int alarmPin = 6;

const int servoPin = 38;

// put function declarations here:
void handleRoot();
void handleRed();
void handleYellow();
void handleGreen();
void handleOff();
void handleAlarm();
void handleKeepAlive();
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void setupWifi(const char *ssid, const char *password, int timeout);
void setupMDNS(String domainName, int timeout);

void setup()
{
  //delay(10000);

  Serial.begin(115200);

  // Initialize GPIO pins

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(amberPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(amberPin, HIGH);
  digitalWrite(alarmPin, HIGH);

  // Mount LittleFS
  while (!LittleFS.begin())
  {
    Serial.println("LittleFS Mount Failed");

    // Check for possible causes
    if (LittleFS.format())
    {
      Serial.println("LittleFS Formatted and Mounted Successfully");
    }
    else
    {
      Serial.println("LittleFS Formatting Failed");
    }
    delay(1000);
  }
  Serial.println("LittleFS mounted successfully");

  // Connect to Wi-Fi
  setupWifi(ssid, password, 10);

  // Set up mDNS
  setupMDNS(hostname, 10);

  // Define routes
  server.on("/", handleRoot); // Serve the HTML page
  server.on("/red", handleRed);
  server.on("/yellow", handleYellow);
  server.on("/green", handleGreen);
  server.on("/off", handleOff);
  server.on("/alarm", handleAlarm);
  server.on("/keep_alive", handleKeepAlive);

  server.begin();

  ms24.setup(servoPin, 270, 500, 2500);

  listDir(LittleFS, "/", 1);

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  Serial.println("get autoconnect"+WiFi.getAutoConnect());
  Serial.println("get autoreconnect"+WiFi.getAutoReconnect());
  Serial.println("get sleep"+WiFi.getSleep());
}

void loop()
{
  server.handleClient();

  // reconnect if connection is lost from wifi
  //if (WiFi.status() != WL_CONNECTED)
  //{
  //  setupWifi(ssid, password, 10);
  //}
  
  // reconnect if connection is lost from mDNS
  //if(!MDNS.begin(hostname)) {
  //  setupMDNS(hostname, 10);
  //}
}

// Function to serve the HTML file
void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    server.send(500, "text/plain", "Failed to open file");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Function to turn on the red light
void handleRed()
{
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(greenPin, HIGH);
  server.send(200, "text/plain", "Red Light ON");
  ms24.setAngle(0);
  Serial.println("Red Button Pressed");
}

// Function to turn on the yellow light
void handleYellow()
{
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, HIGH);
  server.send(200, "text/plain", "Yellow Light ON");
  ms24.setAngle(90);
  Serial.println("Yellow Button Pressed");
}

// Function to turn on the green light
void handleGreen()
{
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(greenPin, LOW);
  server.send(200, "text/plain", "Green Light ON");
  ms24.setAngle(180);
  Serial.println("Green Button Pressed");
}

// Function to turn off the lights
void handleOff()
{
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, HIGH);
  digitalWrite(greenPin, HIGH);
  server.send(200, "text/plain", "Lights Turned OFF");
  Serial.println("Off Button Pressed");
}

// Function to handle alarm
void handleAlarm()
{
  digitalWrite(alarmPin, LOW); // Activate the alarm speaker
  digitalWrite(amberPin, LOW); // Activate the amber light
  digitalWrite(redPin, HIGH);  
  digitalWrite(yellowPin, HIGH); 
  digitalWrite(greenPin, HIGH); 
  server.send(200, "text/plain", "Alarm Activated");
  Serial.println("Alarm Button Pressed");
}

// Function to keep the connection alive
void handleKeepAlive()
{
  server.send(200, "text/plain", "Keep Alive");
  Serial.println("Keep Alive");
}

// Function to list files in the LittleFS recursively down levels
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  Serial.printf("Done listing directory: %s\n", dirname);
}

// Function to setup Wi-Fi
// @param timeout: time in seconds to wait for connection
void setupWifi(const char *ssid, const char *password, int timeout)
{
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.printf("Connecting to WiFi %i/%i\n", i, timeout);
    i++;
    if (i > timeout)
    {
      Serial.println("Connection timed out");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  }
}

// Function to setup mDNS
void setupMDNS(String domainName, int timeout)
{
  // Set up mDNS
  MDNS.begin(domainName);
  MDNS.addService("http", "tcp", 80);
  int i = 0;
  while (!MDNS.begin(domainName))
  {
    delay(1000);
    Serial.println("Error setting up MDNS responder!");
    if (i > timeout)
    {
      Serial.println("mDNS responder timed out");
      break;
    }
  }
  if (MDNS.begin(domainName))
  {
    Serial.println("mDNS responder started");
    //Serial.println(MDNS.port()); //?
  }
}
