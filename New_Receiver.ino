#include <SoftwareSerial.h>
#include <NewPing.h>
#include <Servo.h>

SoftwareSerial BTSerial(10, 11); // Bluetooth RX -> Pin 11, TX -> Pin 10

// Car motor control pins
const int motorInput1 = 5;
const int motorInput2 = 6;
const int motorInput3 = 3;
const int motorInput4 = 9;

// Obstacle detection variables
#define trigPin A1
#define echoPin A2
#define maxDistance 200
const int obstacleThreshold = 30;
NewPing sonar(trigPin, echoPin, maxDistance);

// Servo setup for ultrasonic sensor
Servo ultrasonicServo;
const int servoPin = 8;

// Bluetooth direction command
char tiltDirection;

void setup() {
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
  
  ultrasonicServo.attach(servoPin);
  ultrasonicServo.write(90);  // Start centered
  
  Serial.begin(38400);
  BTSerial.begin(9600);
}

// Main loop
void loop() {
  // Listen for Bluetooth commands
  if (BTSerial.available()) {
    tiltDirection = BTSerial.read();
    Serial.println(tiltDirection);
    if (tiltDirection == 'F') {
      Serial.println("Forward");
      ultrasonicServo.write(90); 
      delay(5);  // Allow time for servo to move
      checkForObstaclesAndMove(); // Check for obstacles before moving
    } else if (tiltDirection == 'B') {
      Serial.println("Reverse");
      //ultrasonicServo.write(270); // Center for reverse
      //delay(10);
      checkForObstaclesAndMove();
    } else if (tiltDirection == 'R') {
      Serial.println("Right");
      ultrasonicServo.write(0); // Turn right
      delay(5);
      checkForObstaclesAndMove();
    } else if (tiltDirection == 'L') {
      Serial.println("Left");
      ultrasonicServo.write(180); // Turn left
      delay(5);
      checkForObstaclesAndMove();
    } else if (tiltDirection == 'S') {
      Serial.println("Stop");
      stopCar();
    }
  }
}

// Function to check for obstacles and move the car
void checkForObstaclesAndMove() {
  int distance = sonar.ping_cm(); // Get distance from ultrasonic sensor
  Serial.println(distance); // Debugging output

  if (distance <= obstacleThreshold && distance > 0) { // Ensure distance is valid
    Serial.println("Obstacle detected, stopping car.");
    stopCar(); // Stop if an obstacle is detected
  } else {
    moveCar(); // Move if no obstacle is detected
  }
}

// Function to move the car based on the last direction command
void moveCar() {
  if (tiltDirection == 'F') {
    Serial.println("Moving Forward");
    forward();
  } else if (tiltDirection == 'B') {
    Serial.println("Moving Forward");
    reverse();
  } else if (tiltDirection == 'R') {
    right();
  } else if (tiltDirection == 'L') {
    left();
  }
}

// Movement functions
void forward() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void reverse() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void right() {
  digitalWrite(motorInput1, LOW);
  analogWrite(motorInput2, 150);
  analogWrite(motorInput3, 150);
  digitalWrite(motorInput4, LOW);
}

void left() {
  analogWrite(motorInput1, 150);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  analogWrite(motorInput4, 150);
}

void stopCar() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
