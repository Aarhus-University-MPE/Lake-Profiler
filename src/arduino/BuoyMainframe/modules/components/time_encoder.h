#pragma once
#include "../setup/modules.h"

int timeMotorDirection          = 0;
unsigned long motorMoveStart    = 0;
unsigned long motorMoveStartPos = 0;
unsigned long currentTimePos    = 0;
long motorCompensation          = 0;

void TimeEncoderActivate() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_MODE, ENCODERMODE_TIME);
  DEBUG_PRINTLN(F("Encoder Mode: Time"));
}

void SetTimeEncoderCompensationScale(float scale) {
  EEPROM.put(MEMADDR_TIMEENCODER_COMPENSATION_SCALE, scale);
  DEBUG_PRINT(F("Setting Time Encoder Compensation Scale: "));
  DEBUG_PRINTLN(scale);
}

float GetTimeEncoderCompensationScale() {
  float compensationScale;
  EEPROM.get(MEMADDR_TIMEENCODER_COMPENSATION_SCALE, compensationScale);
  return compensationScale;
}

void SetTimeEncoderMotorSpeed(float speed) {
  EEPROM.put(MEMADDR_MOTOR_SPEED, speed);
  DEBUG_PRINT(F("Setting Motor Speed: "));
  DEBUG_PRINTLN(speed);
}

float GetTimeEncoderMotorSpeed() {
  float motorSpeed;
  EEPROM.get(MEMADDR_MOTOR_SPEED, motorSpeed);
  return motorSpeed;
}

void UpdateMotorCompensation(uint8_t dir) {
  DEBUG_PRINTLN(F("Updating Motor Compensation"));
  if (dir == MOTOR_DIR_DOWN || dir == MOTOR_DIR_HALT) {
    motorCompensation = 0;
    return;
  }

  unsigned long targetPos = GetTargetTimePosition(dir);
  long moveDistance       = targetPos - currentTimePos;

  motorCompensation = max(0, moveDistance) * GetTimeEncoderCompensationScale();

  DEBUG_PRINT(targetPos);
  DEBUG_PRINT(F(" - \t Distance: "));
  DEBUG_PRINT(moveDistance);
  DEBUG_PRINT(F(" - \t Compensation: "));
  DEBUG_PRINTLN(motorCompensation);
}

unsigned long GetTargetTimePosition(uint8_t dir) {
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetTimeEncoderTop();
      break;
    case MOTOR_DIR_DOWN:
      return GetTimeEncoderBottom();
      break;
    case MOTOR_DIR_SERVICE:
      return GetTimeEncoderService();
      break;
    default:
      return 0;
      break;
  }
}

void TimeEncoderStart(uint8_t dir) {
  // TimeEncoderUpdate();

  motorMoveStart    = millis();
  motorMoveStartPos = currentTimePos;

  UpdateMotorCompensation(dir);
}

// Update time encoder current position and save to EEPROM
void TimeEncoderUpdate() {
  long positionChange = (millis() - motorMoveStart) * GetTimeMotorDirection();

  currentTimePos = motorMoveStartPos + positionChange;

  // DEBUG_PRINT(F("motorMoveStartPos: "));
  // DEBUG_PRINT(motorMoveStartPos);
  // DEBUG_PRINT(F("\t motorMove Start: "));
  // DEBUG_PRINT(motorMoveStart);
  // DEBUG_PRINT(F("\t positionChange: "));
  // DEBUG_PRINT(positionChange);
  // DEBUG_PRINT(F("\t currentTimePos: "));
  // DEBUG_PRINTLN(currentTimePos);

  EEPROMSetTimeMotorPos();
}

// Return current motor direction identifier
int GetTimeMotorDirection() {
  switch (MotorState()) {
    case MOTOR_DIR_DOWN:
      timeMotorDirection = -1;
      break;
    case MOTOR_DIR_UP:
      timeMotorDirection = 1;
      break;
    case MOTOR_DIR_SERVICE:
      timeMotorDirection = 1;
      break;
    case MOTOR_DIR_HALT:
      timeMotorDirection = 0;
      break;
    default:
      break;
  }

  return timeMotorDirection;
}

unsigned long GetTimeEncoderPosition() {
  TimeEncoderUpdate();
  return currentTimePos;
}

unsigned long GetTimeEncoderTop() {
  return TIME_ENCODER_TOP_POSITION;
}

unsigned long GetTimeEncoderBottom() {
  unsigned long timeEncoderValue;
  EEPROM_READ_ULONG(MEMADDR_TIMEENCODER_BOTTOM, timeEncoderValue);
  return timeEncoderValue;
}
unsigned long GetTimeEncoderService() {
  unsigned long timeEncoderValue;
  EEPROM_READ_ULONG(MEMADDR_TIMEENCODER_SERVICE, timeEncoderValue);
  return timeEncoderValue;
}

void SetTimeEncoderBottom(unsigned long value) {
  DEBUG_PRINTLN(value);
  EEPROM.put(MEMADDR_TIMEENCODER_BOTTOM, value);
}

void SetTimeEncoderService(unsigned long value) {
  EEPROM.put(MEMADDR_TIMEENCODER_SERVICE, value);
}

void SetTimeEncoderTop() {
  DEBUG_PRINTLN(F("Setting Top Position - TimeEncoder"));
  currentTimePos    = TIME_ENCODER_TOP_POSITION;
  motorMoveStartPos = currentTimePos;
  motorMoveStart    = millis();
}

// Save current position as bottom position
void SetTimeEncoderBottom() {
  DEBUG_PRINT(F("Setting Bottom Position: "));
  TimeEncoderPrintPos();

  SetTimeEncoderBottom(currentTimePos);
}

// Save bottom position as offset from top position, depth in meters
void SetTimeEncoderDepth(float depth) {
  unsigned long depthChange = depth / GetTimeEncoderMotorSpeed() * 100000UL;

  unsigned long depthValue = GetTimeEncoderTop() - depthChange;

  SetTimeEncoderBottom(depthValue);

  DEBUG_PRINT(F("Setting Bottom Position: "));
  DEBUG_PRINT(depth);
  DEBUG_PRINT(F(" m - \t"));
  DEBUG_PRINT(depthChange);
  DEBUG_PRINTLN(F(" ms from top"));
}

void SetTimeEncoderServiceDepth(float depth) {
  unsigned long depthChange = depth / GetTimeEncoderMotorSpeed() * 100000UL;

  unsigned long depthValue = GetTimeEncoderTop() - depthChange;

  SetTimeEncoderService(depthValue);

  DEBUG_PRINT(F("Setting Service Position: "));
  DEBUG_PRINT(depth);
  DEBUG_PRINT(F(" m - \t"));
  DEBUG_PRINT(depthChange);
  DEBUG_PRINTLN(F(" ms from top"));
}

// Save current position in EEPROM
void EEPROMSetTimeMotorPos() {
  EEPROM.put(MEMADDR_TIMEENCODER_CURRENT, currentTimePos);
  // TimeEncoderPrintPos();
}

// Prints current position
void TimeEncoderPrintPos() {
  if (GetEncoderMode() != ENCODERMODE_TIME) return;
  DEBUG_PRINT(F("Time Encoder Position: "));
  DEBUG_PRINTLN(GetTimeEncoderPosition());
}

// Prints current position
void TimeEncoderPrintTarget(uint8_t dir) {
  if (GetEncoderMode() != ENCODERMODE_TIME) return;
  DEBUG_PRINT(F("Target: "));
  DEBUG_PRINT(GetTargetTimePosition(dir));
  DEBUG_PRINT(F(" - \t Compensation: "));
  DEBUG_PRINT(motorCompensation);
  DEBUG_PRINT(F(" ms - \t Distance: : "));
  if (dir == MOTOR_DIR_DOWN) {
    DEBUG_PRINTLN(GetTimeEncoderPosition() - GetTimeEncoderTarget(dir));
  } else {
    DEBUG_PRINTLN(GetTimeEncoderTarget(dir) - GetTimeEncoderPosition());
  }
}

void TimeEncoderPrintPos(uint8_t direction) {
  if (GetEncoderMode() != ENCODERMODE_TIME) return;
  switch (direction) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Top Position: 0"));
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINT(F("Bottom Position: "));
      DEBUG_PRINTLN(GetTimeEncoderBottom());
      break;
    case MOTOR_DIR_SERVICE:
      DEBUG_PRINT(F("Service Position: "));
      DEBUG_PRINTLN(GetTimeEncoderService());
      break;
    default:
      break;
  }
}

// Read latest position in EEPROM
void EEPROMGetTimeMotorPos() {
  EEPROM.get(MEMADDR_TIMEENCODER_CURRENT, currentTimePos);
  motorMoveStartPos = currentTimePos;
  // EncoderPrintPos();
}

// Returns true if motor has reached time based Encoder Positions
bool MotorPositionReachedTimeEncoder(uint8_t dir) {
  // Sensor Depth Check TODO: Redundancy checking
  // if (LoggingActive() && dir == MOTOR_DIR_UP) {
  //   bool positionReached = GetLatestDepth() >= GetDepthSensorTopPosition();
  //   if (positionReached) SetTimeEncoderTop();
  //   return positionReached;
  // }
  unsigned long currentPosition = GetTimeEncoderPosition();
  unsigned long endPosition     = 0UL;

  bool positionReached = false;

  // Standard Motor Position Checker
  switch (dir) {
    case MOTOR_DIR_UP:
      endPosition     = GetTimeEncoderTop() + motorCompensation;
      positionReached = currentPosition >= endPosition;
      // Set top position (remove compensation overshoot)
      if (positionReached) SetTimeEncoderTop();
      break;
    case MOTOR_DIR_SERVICE:
      endPosition     = GetTimeEncoderService() + motorCompensation;
      positionReached = currentPosition >= endPosition;
      // Set top position (remove compensation overshoot)
      if (positionReached) SetTimeEncoderTop();
      break;
    case MOTOR_DIR_DOWN:
      endPosition     = GetTimeEncoderBottom();
      positionReached = currentPosition <= endPosition;
      break;
    default:
      break;
  }

  return positionReached;
}

unsigned long GetTimeEncoderTarget(uint8_t dir) {
  // Standard Motor Position Checker
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetTimeEncoderTop() + motorCompensation;
      break;
    case MOTOR_DIR_DOWN:
      return GetTimeEncoderBottom();
      break;
    case MOTOR_DIR_SERVICE:
      return GetTimeEncoderService() + motorCompensation;
      break;
    default:
      break;
  }
}

void SetDepthSensorTopPosition(int topDepth) {
  DEBUG_PRINT(F("Setting Depth Sensor Top Position: "));
  DEBUG_PRINTLN(topDepth);

  EEPROM.put(MEMADDR_DEPTH_TOP_POSITION, topDepth);
}

long GetDepthSensorTopPosition() {
  unsigned long sensorPosition;
  EEPROM.get(MEMADDR_DEPTH_TOP_POSITION, sensorPosition);

  return sensorPosition;
}