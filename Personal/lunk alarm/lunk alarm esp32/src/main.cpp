#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h> // for not having to have the raw html in the main
#include "servointerface.h"
#include <ESPmDNS.h> // Include the mDNS library to maintain a local hostname making it accessible via domain name rather than IP address
#include <Adafruit_NeoPixel.h>

// Can I make it its own WAP?
// WiFi.softAP(ssid, password);

// TODO: REPLACE THE CONSTS WITH DEFINES

const char *ssid = "The boys";
const char *password = "607AFA!!";
const String hostname = "lightcontrol"; // The hostname to use for mDNS, will be accessible via lightcontrol.local on the network above

// Create a web server on port 80
WebServer server(80);

// Servo to test while having no test LEDs
ServoInterface ms24;

// Define GPIO pins
const int redPin = 4;
const int yellowPin = 5;
const int greenPin = 6;
const int alarmPin = 7;


const int servoPin = 19;

//const int digitalLEDPin = 8; // digital rgb pin 8 or 11???
// Define the NeoPixel strip
#define PIN 8
#define NUMPIXELS 1

Adafruit_NeoPixel rgb_strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



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
String getLocalIPAddress();
void handleIpRequest();


void setup()
{
  // delay(10000);

  Serial.begin(115200);

  // Initialize GPIO pins

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  rgb_strip.begin();

  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(alarmPin, LOW);
  digitalWrite(alarmPin, LOW);
  rgb_strip.setPixelColor(0, rgb_strip.Color(0, 0, 0));
  delay(500);
  rgb_strip.setPixelColor(0, rgb_strip.Color(10, 0, 0));
  rgb_strip.show();
  delay(500);
  rgb_strip.setPixelColor(0, rgb_strip.Color(0, 10, 0));
  rgb_strip.show();
  delay(500);
  rgb_strip.setPixelColor(0, rgb_strip.Color(0, 0, 10));
  rgb_strip.show();
  delay(500);
  rgb_strip.setPixelColor(0, rgb_strip.Color(0, 0, 0));
  rgb_strip.show();
  delay(500);

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
  server.on("/ip", handleIpRequest);

  server.begin();

  ms24.setup(servoPin, 270, 500, 2500);

  listDir(LittleFS, "/", 1);

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);

  Serial.println("Setup complete");

 // Bootup communication sequence
  digitalWrite(redPin, HIGH);
  delay(500);
  digitalWrite(redPin, LOW);
  delay(500);
  digitalWrite(yellowPin, HIGH);
  delay(500);
  digitalWrite(yellowPin, LOW);
  delay(500);
  digitalWrite(greenPin, HIGH);
  delay(500);
  digitalWrite(greenPin, LOW);
  delay(500);
  digitalWrite(alarmPin, HIGH);
  delay(500);
  digitalWrite(alarmPin, LOW);
}

void loop()
{
  server.handleClient();

  // reconnect if connection is lost from wifi
  if (WiFi.status() != WL_CONNECTED)
  {
    setupWifi(ssid, password, 10);
  }

  // reconnect if connection is lost from mDNS
  // if(!MDNS.begin(hostname)) {
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
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(alarmPin, LOW);
  digitalWrite(redPin, HIGH);
  server.send(200, "text/plain", "Red Light ON");
  ms24.setAngle(0);
  Serial.println("Red Button Pressed");
}

// Function to turn on the yellow light
void handleYellow()
{
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(alarmPin, LOW);
  digitalWrite(yellowPin, HIGH);
  server.send(200, "text/plain", "Yellow Light ON");
  ms24.setAngle(90);
  Serial.println("Yellow Button Pressed");
}

// Function to turn on the green light
void handleGreen()
{
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(alarmPin, LOW);
  digitalWrite(greenPin, HIGH);
  server.send(200, "text/plain", "Green Light ON");
  ms24.setAngle(180);
  Serial.println("Green Button Pressed");
}

// Function to turn off the lights
void handleOff()
{
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(alarmPin, LOW);
  server.send(200, "text/plain", "Lights Turned OFF");
  Serial.println("Off Button Pressed");
}

// Function to handle alarm
void handleAlarm()
{
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(alarmPin, HIGH); // Activate the alarm speaker
  digitalWrite(redPin, HIGH); // Activate the red light
  delay(1000);
  digitalWrite(alarmPin, LOW); // Deactivate the alarm speaker
  digitalWrite(redPin, LOW); // Deactivate the red light
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
  rgb_strip.setPixelColor(0, rgb_strip.Color(0, 0, 0));
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
    rgb_strip.setPixelColor(0, rgb_strip.Color(0, 10, 0));
    rgb_strip.show();
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
    // Serial.println(MDNS.port()); //?
  }
}

// Function to get the local IP address
String getLocalIpAddress()
{
  return WiFi.localIP().toString();
}

// Function to handle IP request
void handleIpRequest()
{
  server.send(200, "text/plain", getLocalIpAddress());
}