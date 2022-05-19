/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once

#include "../setup/modules.h"

byte motorState;
unsigned long lastMillisRead, setStateStart;
bool setPos = false;

// Initialize motors and turns on encoder power
bool InitializeMotors() {
  // digitalWrite(PO_POWER_ENCODER, true); // Powered on by default

  // Initialize outputs
  digitalWrite(PO_MOTOR_DOWN, false);
  digitalWrite(PO_MOTOR_UP, false);

  setPos = false;

  EEPROMGetMotorPos();
  return true;
}

// Disables motors and turns off encoder power
void TerminateMotors() {
  // Save current position to EEPROM
  EEPROMSetMotorPos();
  // Power off encoder
  // digitalWrite(PO_POWER_ENCODER, false); // // Powered on by default

  // Motor termination
  digitalWrite(PO_MOTOR_DOWN, false);
  digitalWrite(PO_MOTOR_UP, false);
}

// Returns true if motor is moving
bool MotorState() {
  return digitalRead(PO_MOTOR_DOWN) || digitalRead(PO_MOTOR_UP);
}

// Moves motor based on input buttons
void MotorProcess() {
  if (millis() - lastMillisRead < BUTTON_DBOUNCE_TIME) return;

  lastMillisRead = millis();

  if (setPos) MotorSetPos();

  // Both Buttons Pressed
  else if (!digitalRead(PI_BUTTON_MOTOR_UP) && !digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    MotorMove(MOTOR_DIR_HALT);
    MotorSetState();
  }
  // Up Button Pressed
  else if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
    MotorMove(MOTOR_DIR_UP);
    delay(250);
  }
  // Down Button Pressed
  else if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    MotorMove(MOTOR_DIR_DOWN);
    delay(250);
  }
  // End position reached
  else if (MotorPositionReached() && motorState != MOTOR_DIR_HALT) {
    DEBUG_PRINTLN(F("End position reached!"));
    MotorMove(MOTOR_DIR_HALT);
  }
}

// Move motors based on input, if
void MotorMove(uint8_t dir) {
  if (dir == motorState) return;

  // Save current position to EEPROM
  EEPROMSetMotorPos();

  if (motorState == MOTOR_DIR_UP || motorState == MOTOR_DIR_DOWN) {
    digitalWrite(PO_MOTOR_DOWN, false);
    digitalWrite(PO_MOTOR_UP, false);
    DEBUG_PRINTLN(F("Moving Halt"));
    motorState = MOTOR_DIR_HALT;
    // delay(500);
    return;
  }

  EncoderPrintPos();

  motorState = dir;
  switch (dir) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Moving towards: "));
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      digitalWrite(LED_BUILTIN, true);
      EncoderPrintPos(dir);
      break;
    case MOTOR_DIR_DOWN:
      digitalWrite(PO_MOTOR_DOWN, true);
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(LED_BUILTIN, true);
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      break;
    case MOTOR_DIR_HALT:
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(PO_MOTOR_DOWN, false);
      DEBUG_PRINTLN(F("Moving Halt"));
      digitalWrite(LED_BUILTIN, false);
      break;
    default:
      break;
  }
}

// Next button input (longer than 500 ms) will save that direction
void MotorSetPos() {
  if (millis() - setStateStart > TIMEOUT_SET_POS) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Set State TIMEOUT"));
    DEBUG_PRINTLINE();
    setPos = false;
  }
  if (!digitalRead(PI_BUTTON_MOTOR_DOWN) && !digitalRead(PI_BUTTON_MOTOR_UP)) return;

  if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
    delay(500);
    if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
      SetEncoderTop();
      setPos = false;
    }
  } else if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    delay(500);
    if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
      SetEncoderBottom();
      setPos = false;
    }
  }
}

void MotorSetState() {
  delay(2000);

  // Both Buttons Still Pressed?
  if (!digitalRead(PI_BUTTON_MOTOR_UP) && !digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Set State, next button press sets new endpositions"));
    DEBUG_PRINTLINE();
    setPos        = true;
    setStateStart = millis();
  }
}

// Returns true if endposition reached for specified direction
bool MotorPositionReached(uint8_t dir) {
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetEncoderRotations() >= GetEncoderRotationsTop() && GetEncoderCount() >= GetEncoderCountTop();
      break;
    case MOTOR_DIR_DOWN:
      return GetEncoderRotations() <= GetEncoderRotationsBottom() && GetEncoderCount() <= GetEncoderCountBottom();
      break;
    default:
      break;
  }
}

// Returns true if endposition reached for specified direction
bool MotorPositionReached() {
  return MotorPositionReached(motorState);
}

// Motors operational?
bool MotorStatus() {
  return GetStatus(MODULE_MOTOR) && GetStatus(MODULE_PWR_MOTOR);
}
