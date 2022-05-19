#pragma once
#include "../setup/modules.h"

int motorDirection   = 1;
int encoderCount     = 0;
int encoderRotations = 0;

// Increments encoder values
void EncoderAInterrupt() {
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
  encoderRotations += motorDirection;
  encoderCount = 0;
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

int GetEncoderCountTop() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_COUNT_TOP);
}
int GetEncoderCountBottom() {
  return EEPROM_READ_INT(MEMADDR_ENCODER_COUNT_BOTTOM);
}

void SetEncoderRotationsTop(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_TOP, value);
}
void SetEncoderRotationsBottom(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_BOTTOM, value);
}

void SetEncoderCountTop(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_TOP, value);
}
void SetEncoderCountBottom(int value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_BOTTOM, value);
}

// Save current position as top position
void SetEncoderTop() {
  DEBUG_PRINT(F("Setting Top Position - "));
  EncoderPrintPos();

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

    default:
      break;
  }
}

// Read latest position in EEPROM
void EEPROMGetMotorPos() {
  encoderCount     = EEPROM_READ_INT(MEMADDR_ENCODER_COUNT);
  encoderRotations = EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION);
}