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
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("Setting Top Position"));
  DEBUG_PRINTLINE();
  // EncoderPrintPos();

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

// Prints current position
void EncoderPrintPos() {
  DEBUG_PRINT(F("Encoder Position, Count: "));
  DEBUG_PRINT(encoderCount);
  DEBUG_PRINT(F(", Rotations: "));
  DEBUG_PRINTLN(encoderRotations);
}

void EncoderPrintPos(uint8_t direction) {
  switch (direction) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Top Position - Count: "));
      DEBUG_PRINT(GetEncoderCountTop());
      DEBUG_PRINT(F(", Rotations: "));
      DEBUG_PRINTLN(GetEncoderRotationsTop());
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINT(F("Bottom Position - Count: "));
      DEBUG_PRINT(GetEncoderCountBottom());
      DEBUG_PRINT(F(", Rotations: "));
      DEBUG_PRINTLN(GetEncoderRotationsBottom());
      break;
    case MOTOR_DIR_SERVICE:
      DEBUG_PRINT(F("Service Position - Count: "));
      DEBUG_PRINT(GetEncoderCountService());
      DEBUG_PRINT(F(", Rotations: "));
      DEBUG_PRINTLN(GetEncoderRotationsService());
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