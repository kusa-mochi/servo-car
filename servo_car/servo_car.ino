#include <Servo.h>
#define MAX_SERVO_VALUE 180
#define MIN_SERVO_VALUE 0
#define STOP_SERVO_VALUE 90
#define DISTANCE_THRESHOLD 100
#define TRUE 1
#define FALSE 0

typedef int BOOL;

Servo leftServo, rightServo;

// pins
int leftServoPin = 3;
int rightServoPin = 5;
int frontLeftDistanceSensorPin = A0;
int frontRightDistanceSensorPin = A1;
int rearLeftDistanceSensorPin = A2;
int rearRightDistanceSensorPin = A3;

int sleepDuration = 500;
int turnDuration = 2000;
// int frontLeftDistance = 9999;
// int frontRightDistance = 9999;
// int rearLeftDistance = 9999;
// int rearRightDistance = 9999;

enum SpeedLevel {
  Stop,
  Low,
  Middle,
  High
};

// Wheel Controls

void leftWheelForward(SpeedLevel speed) {
  switch (speed) {
    case SpeedLevel::Stop:
      leftServo.write(STOP_SERVO_VALUE);
      break;
    case SpeedLevel::Low:
      leftServo.write(120);
      break;
    case SpeedLevel::Middle:
      leftServo.write(150);
      break;
    case SpeedLevel::High:
      leftServo.write(180);
      break;
    default:
      leftServo.write(STOP_SERVO_VALUE);
      break;
  }
}

void rightWheelForward(SpeedLevel speed) {
  switch (speed) {
    case SpeedLevel::Stop:
      rightServo.write(STOP_SERVO_VALUE);
      break;
    case SpeedLevel::Low:
      rightServo.write(60);
      break;
    case SpeedLevel::Middle:
      rightServo.write(30);
      break;
    case SpeedLevel::High:
      rightServo.write(0);
      break;
    default:
      rightServo.write(STOP_SERVO_VALUE);
      break;
  }
}

void leftWheelBackward(SpeedLevel speed) {
  switch (speed) {
    case SpeedLevel::Stop:
      leftServo.write(STOP_SERVO_VALUE);
      break;
    case SpeedLevel::Low:
      leftServo.write(60);
      break;
    case SpeedLevel::Middle:
      leftServo.write(30);
      break;
    case SpeedLevel::High:
      leftServo.write(0);
      break;
    default:
      leftServo.write(STOP_SERVO_VALUE);
      break;
  }
}

void rightWheelBackward(SpeedLevel speed) {
  switch (speed) {
    case SpeedLevel::Stop:
      rightServo.write(STOP_SERVO_VALUE);
      break;
    case SpeedLevel::Low:
      rightServo.write(120);
      break;
    case SpeedLevel::Middle:
      rightServo.write(150);
      break;
    case SpeedLevel::High:
      rightServo.write(180);
      break;
    default:
      rightServo.write(STOP_SERVO_VALUE);
      break;
  }
}

void leftWheelStop() {
  leftWheelForward(SpeedLevel::Stop);
}

void rightWheelStop() {
  rightWheelForward(SpeedLevel::Stop);
}

// Car Operations

void goForward(SpeedLevel speed) {
  leftWheelForward(speed);
  rightWheelForward(speed);
}

void goBackward(SpeedLevel speed) {
  leftWheelBackward(speed);
  rightWheelBackward(speed);
}

void turnLeft(SpeedLevel speed) {
  leftWheelBackward(speed);
  rightWheelForward(speed);
  delay(turnDuration);
}

void turnRight(SpeedLevel speed) {
  leftWheelForward(speed);
  rightWheelBackward(speed);
  delay(turnDuration);
}

// Read Distance Sensor Values

BOOL isObstacleInFrontLeft() {
  int distance = analogRead(frontLeftDistanceSensorPin);
  SerialWriteDistance("FL d=", distance);
  if (distance < DISTANCE_THRESHOLD) return TRUE;
  else return FALSE;
}

BOOL isObstableInFrontRight() {
  int distance = analogRead(frontRightDistanceSensorPin);
  SerialWriteDistance("FR d=", distance);
  if (distance < DISTANCE_THRESHOLD) return TRUE;
  else return FALSE;
}

BOOL isObstableInRearLeft() {
  int distance = analogRead(rearLeftDistanceSensorPin);
  SerialWriteDistance("RL d=", distance);
  if (distance < DISTANCE_THRESHOLD) return TRUE;
  else return FALSE;
}

BOOL isObstableInRearRight() {
  int distance = analogRead(rearRightDistanceSensorPin);
  SerialWriteDistance("RR d=", distance);
  if (distance < DISTANCE_THRESHOLD) return TRUE;
  else return FALSE;
}

// Debugging

void SerialWriteDistance(String preString, int distance) {
  Serial.print(preString);
  Serial.print(distance);
  Serial.println();
}

// Entrypoint

void setup() {
  Serial.begin(9600);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  leftServo.write(STOP_SERVO_VALUE);
  rightServo.write(STOP_SERVO_VALUE);
}

void loop() {
  // check distances
  Serial.println("---");
  BOOL obstacleFL = isObstacleInFrontLeft();
  BOOL obstacleFR = isObstableInFrontRight();
  BOOL obstacleRL = isObstableInRearLeft();
  BOOL obstacleRR = isObstableInRearRight();

  // if near by something obstacle,
  if (obstacleFL == TRUE && obstacleFR == TRUE) {
    goBackward(SpeedLevel::Low);
  } else if (obstacleRL == TRUE) {
    turnRight(SpeedLevel::High);
  } else if (obstacleRR == TRUE) {
    turnLeft(SpeedLevel::High);
  } else if (obstacleFL == TRUE) {
    turnRight(SpeedLevel::High);
  } else if (obstacleFR == TRUE) {
    turnLeft(SpeedLevel::High);
  } else {
    goForward(SpeedLevel::High);
  }

  // Serial.println(distance);
  delay(sleepDuration);
}
