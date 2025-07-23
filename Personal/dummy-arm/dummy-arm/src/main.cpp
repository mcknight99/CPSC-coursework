#include <Arduino.h>
#include <Servo.h>
#include <math.h>

// Servo objects
Servo servo1; // Base yaw
Servo servo2; // Shoulder pitch
Servo servo3; // Wrist roll

// Pin definitions
const int servo1Pin = 3;
const int servo2Pin = 5;
const int servo3Pin = 6;

// Placeholder dimensions (replace with actual measurements)
const int baseHeight = 50;  // Height of the base (mm)
const int arm1Length = 100; // Length of the first arm segment (mm)
const int arm2Length = 80;  // Length of the second arm segment (mm)

// Angle limits
const int minAngle = 0;
const int maxAngle = 180;

// Function prototypes
void setup();
void loop();
void moveToPosition(float x, float y, float z);

void setup() {
  // Attach servos to pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  // Set initial positions
  servo1.write(180);

  Serial.begin(9600);

  delay(5000);
  
  // // sweep through X
  // Serial.println("Sweeping through X");
  // for (int i = 0; i < 180; i++) {
  //   moveToPosition(i, 0, 0);
  //   Serial.println(i);
  //   delay(10);
  // }
  // // sweep through Y
  // Serial.println("Sweeping through Y");
  // for (int i = 0; i < 180; i++) {
  //   moveToPosition(0, i, 0);
  //   Serial.println(i);
  //   delay(10);
  // }
  // // sweep through Z
  // Serial.println("Sweeping through Z");
  // for (int i = 0; i < 180; i++) {
  //   moveToPosition(0, 0, i);
  //   Serial.println(i);
  //   delay(10);
  // }
  // // sweep through X, Y, Z
  // Serial.println("Sweeping through X, Y, Z");
  // for (int i = 0; i < 180; i++) {
  //   moveToPosition(i, i, i);
  //   Serial.println(i);
  //   delay(10);
  // }
  // // reverse sweep through X, Y, Z
  // Serial.println("Reverse sweeping through X, Y, Z");
  // for (int i = 180; i > 0; i--) {
  //   moveToPosition(i, i, i);
  //   Serial.println(i);
  //   delay(10);
  // }
}

void loop() {
  // take in serial input to drive each servo or through the 3d space
  // left hand keyboard for 3d space maneuvering, right hand for independent servo control
  // WS for forward/backward inverse kinematics
  // AD for left/right inverse kinematics
  // QE to go up and down inverse kinematics
  // IK to pitch
  // JL to yaw
  // UO to roll


  // quick script to anti idle my laptop using one servo on my mouse
  // servo1.write(180);
  // delay(600000);
  // servo1.write(0);
  // delay(1000);

  
  // // Read serial input
  // if (Serial.available() > 0) {
  //   char input = Serial.read();
  //   switch (input) {
  //     case 'w':
  //       moveToPosition(0, 0, 10);
  //       break;
  //     case 's':
  //       moveToPosition(0, 0, -10);
  //       break;
  //     case 'a':
  //       moveToPosition(10, 0, 0);
  //       break;
  //     case 'd':
  //       moveToPosition(-10, 0, 0);
  //       break;
  //     case 'q':
  //       moveToPosition(0, 10, 0);
  //       break;
  //     case 'e':
  //       moveToPosition(0, -10, 0);
  //       break;
  //     case 'i':
  //       servo2.write(servo2.read() + 3);
  //       break;
  //     case 'k':
  //       servo2.write(servo2.read() - 3);
  //       break;
  //     case 'j':
  //       servo1.write(servo1.read() + 3);
  //       break;
  //     case 'l':
  //       servo1.write(servo1.read() - 3);
  //       break;
  //     case 'u':
  //       servo3.write(servo3.read() + 3);
  //       break;
  //     case 'o':
  //       servo3.write(servo3.read() - 3);
  //       break;
  //     default:
  //       break;
  //   }
  // }

  servo1.write(20); // Click down
  delay(3000); // Wait for 3 seconds
  servo1.write(120); // Unclick to neutral position
  int minutes = 4;
  for(int i = 0; i < minutes; i++) {
    Serial.println("Time until next click: " + String(minutes - i) + " minutes and 45 seconds");
    delay(60000); // Wait for 60 seconds
  }
  Serial.println("Time until next click: 45 seconds");
  delay(45000); // Wait for 45 seconds
}


// Function to solve inverse kinematics and move servos
void moveToPosition(float x, float y, float z) {
  // Base rotation angle (yaw)
  float theta1 = atan2(y, x) * 180.0 / M_PI;

  // Calculate distances
  float planarDistance = sqrt(x * x + y * y);
  float totalDistance = sqrt(planarDistance * planarDistance + (z - baseHeight) * (z - baseHeight));

  // Check if the target is reachable
  if (totalDistance > arm1Length + arm2Length || totalDistance < fabs(arm1Length - arm2Length)) {
    Serial.println("Target position out of reach.");
    return;
  }

  // Solve for joint angles using the cosine rule
  float a = atan2(z - baseHeight, planarDistance);
  float b = acos((arm1Length * arm1Length + totalDistance * totalDistance - arm2Length * arm2Length) / (2 * arm1Length * totalDistance));
  float theta2 = (a + b) * 180.0 / M_PI;

  float c = acos((arm1Length * arm1Length + arm2Length * arm2Length - totalDistance * totalDistance) / (2 * arm1Length * arm2Length));
  float theta3 = (180 - c * 180.0 / M_PI);

  // Move servos
  servo1.write(constrain(theta1, minAngle, maxAngle));
  servo2.write(constrain(theta2, minAngle, maxAngle));
  servo3.write(constrain(theta3, minAngle, maxAngle));

  // Debug output
  Serial.print("Moving to: ");
  Serial.print("Theta1: "); Serial.print(theta1);
  Serial.print(", Theta2: "); Serial.print(theta2);
  Serial.print(", Theta3: "); Serial.println(theta3);
}
