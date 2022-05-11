#pragma once
#include "../setup/modules.h"

byte motorDirection;
int encoderCount;

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

int encoderRotations;
// Increments encoder values
void EncoderZInterrupt() {
  DEBUG_PRINTLN("Z");
  encoderRotations += motorDirection;
  encoderCount = 0;
}

byte GetMotorDirection() {
  return motorDirection;
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