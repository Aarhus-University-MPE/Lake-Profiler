#pragma once
#include "../setup/modules.h"

byte motorDirection;
int encoderCount     = 0;
int encoderRotations = 0;

int encoderRotationsTop    = 0;
int encoderRotationsBottom = 0;

int encoderCountTop    = 0;
int encoderCountBottom = 0;

// Increments encoder values
void EncoderAInterrupt() {
  DEBUG_PRINTLN("A");
  motorDirection = digitalRead(PI_ENCODER_B);
  encoderCount += motorDirection;
}

// Increments encoder values
void EncoderBInterrupt() {
  // motorDirection = digitalRead(PI_ENCODER_A);
  // encoderCount += motorDirection;
}

// Increments encoder values
void EncoderZInterrupt() {
  DEBUG_PRINTLN("Z");
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

void SetEncoderRotationsTop(uint8_t value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_TOP, value);
}
void SetEncoderRotationsBottom(uint8_t value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION_BOTTOM, value);
}

void SetEncoderCountTop(uint8_t value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_TOP, value);
}
void SetEncoderCountBottom(uint8_t value) {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT_BOTTOM, value);
}

// Save current position in EEPROM
void EEPROMSetMotorPos() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT, encoderCount);
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION, encoderRotations);
}

// Read latest position in EEPROM
void EEPROMGetMotorPos() {
  encoderCount     = EEPROM_READ_INT(MEMADDR_ENCODER_COUNT);
  encoderRotations = EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION);
}