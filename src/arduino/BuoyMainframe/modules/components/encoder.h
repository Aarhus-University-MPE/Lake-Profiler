#pragma once
#include "../setup/modules.h"

int motorDirection           = 1;
int encoderCount             = 0;
int encoderRotations         = 0;
unsigned long millisEncoderA = 0;
unsigned long millisEncoderZ = 0;

// Increments encoder values
void EncoderAInterrupt() {
  if (millis() - millisEncoderA < 5) return;
  millisEncoderA = millis();
  motorDirection = digitalRead(PI_ENCODER_B) ? 1 : -1;
  encoderCount += motorDirection;
}

// Increments encoder values
void EncoderBInterrupt() {
  motorDirection = digitalRead(PI_ENCODER_A);
  encoderCount += motorDirection;
}

// Increments encoder values
void EncoderZInterrupt() {
  if (millis() - millisEncoderZ < 50) return;
  millisEncoderZ = millis();
  encoderRotations += motorDirection;
  encoderCount = 0;
}

void EncoderActivate() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_MODE, ENCODERMODE_SICK);
  DEBUG_PRINTLN(F("Encoder Mode: Sick"));
}

// Returns true if encoder rotations are within values
bool EncoderWithinLimits() {
  return encoderRotations <= GetEncoderRotationsTop() && encoderRotations >= GetEncoderRotationsBottom();
}

byte GetMotorDirection() {
  return motorDirection;
}

int GetEncoderCount() {
  return encoderCount;
}

int GetEncoderRotations() {
  return encoderRotations;
}

int GetEncoderRotationsTop() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION_TOP);
}
int GetEncoderRotationsBottom() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION_BOTTOM);
}
int GetEncoderRotationsService() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION_SERVICE);
}

int GetEncoderCountTop() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_COUNT_TOP);
}
int GetEncoderCountBottom() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_COUNT_BOTTOM);
}
int GetEncoderCountService() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_COUNT_SERVICE);
}

void SetEncoderRotationsTop(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_TOP, value);
}
void SetEncoderRotationsBottom(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_BOTTOM, value);
}
void SetEncoderRotationsService(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_SERVICE, value);
}

void SetEncoderCountTop(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_TOP, value);
}
void SetEncoderCountBottom(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_BOTTOM, value);
}
void SetEncoderCountService(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_SERVICE, value);
}

// Save current position as top position
void SetEncoderTop() {
  encoderCount     = 0;
  encoderRotations = 0;

  SetEncoderCountTop(encoderCount);
  SetEncoderRotationsTop(encoderRotations);
}

// Save current position as bottom position
void SetEncoderBottom() {
  DEBUG_PRINT(F("Setting Bottom Position - Count: "));
  EncoderPrintPos();

  SetEncoderCountBottom(encoderCount);
  SetEncoderRotationsBottom(encoderRotations);
}

// Save bottom position as offset from top position
void SetEncoderDepth(float depth) {
  // PI * D * rot = depth
  int depthChangeRotation = (int)(depth / (PI * ENCODER_WHEEL_DIAMETER));

  // 100 counts/rev
  int depthChangeCount = (int)(depth / (PI * ENCODER_WHEEL_DIAMETER) * 100.0f) - depthChangeRotation * 100;

  SetEncoderCountBottom(-depthChangeCount);
  SetEncoderRotationsBottom(-depthChangeRotation);

  DEBUG_PRINT(F("Setting Bottom Position - "));
  EncoderPrintPos(MOTOR_DIR_DOWN);
}

void SetEncoderServiceDepth(float depth) {
  // PI * D * rot = depth
  int depthChangeRotation = (int)(depth / (PI * ENCODER_WHEEL_DIAMETER));

  // 100 counts/rev
  int depthChangeCount = (int)(depth / (PI * ENCODER_WHEEL_DIAMETER) * 100.0f) - depthChangeRotation * 100;

  SetEncoderCountService(-depthChangeCount);
  SetEncoderRotationsService(-depthChangeRotation);

  DEBUG_PRINT(F("Setting Service Position - "));
  EncoderPrintPos(MOTOR_DIR_SERVICE);
}

// Save current position in EEPROM
void EEPROMSetMotorPos() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT, encoderCount);
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION, encoderRotations);
  // EncoderPrintPos();
}

int ConvertEncoderSignal(int rotations, int counts) {
  return rotations * 100 + counts;
}

int GetEncoderPosition() {
  return ConvertEncoderSignal(GetEncoderRotations(), GetEncoderCount());
}

int GetEncoderPositionTop() {
  return ConvertEncoderSignal(GetEncoderRotationsTop(), GetEncoderCountTop());
}

int GetEncoderPositionBottom() {
  return ConvertEncoderSignal(GetEncoderRotationsBottom(), GetEncoderCountBottom());
}

int GetEncoderPositionService() {
  return ConvertEncoderSignal(GetEncoderRotationsService(), GetEncoderCountService());
}

// Prints current position
void EncoderPrintPos() {
  if (GetEncoderMode() != ENCODERMODE_SICK) return;
  DEBUG_PRINT(F("Encoder Position: "));
  DEBUG_PRINTLN(GetEncoderPosition());
  // DEBUG_PRINT(F(", count: "));
  // DEBUG_PRINT(encoderCount);
  // DEBUG_PRINT(F(", Rotations: "));
  // DEBUG_PRINTLN(encoderRotations);
}

void EncoderPrintPos(uint8_t direction) {
  if (GetEncoderMode() != ENCODERMODE_SICK) return;
  switch (direction) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Top Position: "));
      DEBUG_PRINTLN(GetEncoderPositionTop());
      // DEBUG_PRINT(F(", count: "));
      // DEBUG_PRINT(GetEncoderCountTop());
      // DEBUG_PRINT(F(", Rotations: "));
      // DEBUG_PRINT(GetEncoderRotationsTop());
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINT(F("Bottom Position: "));
      DEBUG_PRINTLN(GetEncoderPositionBottom());
      // DEBUG_PRINT(F(",  Count: "));
      // DEBUG_PRINT(GetEncoderCountBottom());
      // DEBUG_PRINT(F(", Rotations: "));
      // DEBUG_PRINT(GetEncoderRotationsBottom());
      break;
    case MOTOR_DIR_SERVICE:
      DEBUG_PRINT(F("Service Position: "));
      DEBUG_PRINTLN(GetEncoderPositionService());
      // DEBUG_PRINT(F(",  Count: "));
      // DEBUG_PRINT(GetEncoderCountService());
      // DEBUG_PRINT(F(", Rotations: "));
      // DEBUG_PRINT(GetEncoderRotationsService());
      break;
    default:
      break;
  }
}

// Read latest position in EEPROM
void EEPROMGetMotorPos() {
  encoderCount     = EEPROM_READ_INT(MEMADDR_ENCODER_COUNT);
  encoderRotations = EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION);
  // EncoderPrintPos();
}

bool MotorPositionReachedEncoder(uint8_t dir) {
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetEncoderPosition() >= GetEncoderPositionTop();
      break;
    case MOTOR_DIR_DOWN:
      return GetEncoderPosition() <= GetEncoderPositionBottom();
      break;
    case MOTOR_DIR_SERVICE:
      return GetEncoderPosition() >= GetEncoderPositionService();
      break;
    default:
      break;
  }
}