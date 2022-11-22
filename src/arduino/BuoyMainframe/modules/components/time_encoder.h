#pragma once
#include "../setup/modules.h"

int timeMotorDirection          = 0;
unsigned long motorMoveStart    = 0;
unsigned long motorMoveStartPos = 0;
unsigned long currentTimePos    = 0;
unsigned long motorCompensation = 0;

void TimeEncoderActivate() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_MODE, ENCODERMODE_TIME);
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

void UpdateMotorCompensation() {
  if (MotorState() == MOTOR_DIR_DOWN || MotorState() == MOTOR_DIR_HALT) {
    motorCompensation = 0;
    return;
  }

  unsigned long targetPos    = GetTargetTimePosition();
  unsigned long moveDistance = targetPos - currentTimePos;

  motorCompensation = moveDistance * GetTimeEncoderCompensationScale();
}

unsigned long GetTargetTimePosition() {
  switch (MotorState()) {
    case MOTOR_DIR_UP:
      return GetTimeEncoderTop();
      break;
    case MOTOR_DIR_DOWN:
      return GetTimeEncoderTop();
      break;
    case MOTOR_DIR_SERVICE:
      return GetTimeEncoderTop();
      break;
    default:
      return 0;
      break;
  }
}

void TimeEncoderStart() {
  TimeEncoderUpdate();

  motorMoveStart    = millis();
  motorMoveStartPos = currentTimePos;

  UpdateMotorCompensation();
}

// Update time encoder current position and save to EEPROM
void TimeEncoderUpdate() {
  unsigned long positionChange = (millis() - motorMoveStart) * GetTimeMotorDirection();

  currentTimePos = motorMoveStartPos + positionChange;
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
  return currentTimePos;
}

unsigned long GetTimeEncoderTop() {
  return TIME_ENCODER_TOP_POSITION;
}

unsigned long GetTimeEncoderBottom() {
  unsigned long timeEncoderValue;
  EEPROM_READ_ULONG(MEMADDR_ENCODER_ROTATION_BOTTOM, timeEncoderValue);
  return timeEncoderValue;
}
unsigned long GetTimeEncoderService() {
  unsigned long timeEncoderValue;
  EEPROM_READ_ULONG(MEMADDR_ENCODER_ROTATION_SERVICE, timeEncoderValue);
  return timeEncoderValue;
}

void SetTimeEncoderBottom(unsigned long value) {
  EEPROM.put(MEMADDR_ENCODER_ROTATION_BOTTOM, value);
}

void SetTimeEncoderService(unsigned long value) {
  EEPROM.put(MEMADDR_ENCODER_ROTATION_SERVICE, value);
}

void SetTimeEncoderTop() {
  currentTimePos = TIME_ENCODER_TOP_POSITION;
}

// Save current position as bottom position
void SetTimeEncoderBottom() {
  DEBUG_PRINT(F("Setting Bottom Position: "));
  TimeEncoderPrintPos();

  SetTimeEncoderBottom(currentTimePos);
}

// Save bottom position as offset from top position, depth in meters
void SetTimeEncoderDepth(float depth) {
  unsigned long depthChange = depth / MOTOR_SPEED * 1000000;

  unsigned long depthValue = GetTimeEncoderTop() - depthChange;

  SetTimeEncoderBottom(depthValue);

  DEBUG_PRINT(F("Setting Bottom Position - "));
  TimeEncoderPrintPos(MOTOR_DIR_DOWN);
}

void SetTimeEncoderServiceDepth(float depth) {
  unsigned long depthChange = depth / MOTOR_SPEED * 1000000;

  unsigned long depthValue = GetTimeEncoderTop() - depthChange;

  SetTimeEncoderService(depthValue);

  DEBUG_PRINT(F("Setting Service Position - "));
  TimeEncoderPrintPos(MOTOR_DIR_SERVICE);
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
  currentTimePos = EEPROM_READ_UINT(MEMADDR_TIMEENCODER_CURRENT);
  // EncoderPrintPos();
}

// Returns true if motor has reached time based Encoder Positions
bool MotorPositionReachedTimeEncoder(uint8_t dir) {
  // Sensor Depth Check TODO: Redundancy checking
  if (LoggingActive() && dir == MOTOR_DIR_UP) {
    bool positionReached = GetLatestDepth() >= GetDepthSensorTopPosition();
    if (positionReached) SetTimeEncoderTop();
    return positionReached;
  }

  // Standard Motor Position Checker
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetTimeEncoderPosition() >= GetTimeEncoderTop() + motorCompensation;
      break;
    case MOTOR_DIR_DOWN:
      return GetTimeEncoderPosition() <= GetTimeEncoderBottom();
      break;
    case MOTOR_DIR_SERVICE:
      return GetTimeEncoderPosition() >= GetTimeEncoderService() + motorCompensation;
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