#pragma once
#include "../setup/modules.h"

byte motorDirection;
int encoderCount     = 0;
int encoderRotations = 0;

unsigned int lastMillisUpdate = 0;

// Update and save current positions
void EncoderUpdate() {
  // Throttle updates
  if (millis() - lastMillisUpdate < ENCODER_UPDATE_PERIOD) return;

  lastMillisUpdate = millis();
  EEPROMSetMotorPos();
}

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

// Save current position in EEPROM TODO: Add this somewhere
void EEPROMSetMotorPos() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT, encoderCount);
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION, encoderRotations);
}

// Prints current position
void EncoderPrintPos() {
  DEBUG_PRINT(F("Count: "));
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