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

  // Both Buttons Pressed
  if (!digitalRead(PI_BUTTON_MOTOR_UP) && !digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    MotorMove(MOTOR_DIR_HALT);
    delay(3000);
    if (!digitalRead(PI_BUTTON_MOTOR_UP) && !digitalRead(PI_BUTTON_MOTOR_DOWN)) {
      SetEncoderTop();
    }
  }
  // Up Button Pressed
  else if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
    if (motorState == MOTOR_DIR_DOWN) {
      MotorMove(MOTOR_DIR_HALT);
    } else {
      MotorMove(MOTOR_DIR_SERVICE);
    }
    delay(500);
  }
  // Down Button Pressed
  else if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    if (motorState == MOTOR_DIR_SERVICE || motorState == MOTOR_DIR_UP) {
      MotorMove(MOTOR_DIR_HALT);
    } else {
      MotorMove(MOTOR_DIR_DOWN);
    }
    delay(500);
  }
  // End position reached
  else if (motorState != MOTOR_DIR_HALT) {
    if (MotorPositionReached()) {
      DEBUG_PRINTLN(F("End position reached!"));
      MotorMove(MOTOR_DIR_HALT);
    }
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
    // DEBUG_PRINTLN(F("Moving Halt"));
    motorState = MOTOR_DIR_HALT;
    // delay(500);
    return;
  }

  // Print current encoder position
  // EncoderPrintPos();

  motorState = dir;
  DEBUG_PRINT(F("Current Position: "));
  EncoderPrintPos();

  switch (dir) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      digitalWrite(PO_MOTOR_DOWN, true);
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_SERVICE:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_HALT:
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(PO_MOTOR_DOWN, false);
      // DEBUG_PRINTLN(F("Moving Halt"));
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

unsigned long millisPrintMotorPos = 0;

// Returns true if endposition reached for specified direction
bool MotorPositionReached(uint8_t dir) {
  if (millis() - millisPrintMotorPos > 5000) {
    millisPrintMotorPos = millis();
    // EncoderPrintPos();
  }
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

// Returns true if endposition reached for specified direction
bool MotorPositionReached() {
  return MotorPositionReached(motorState);
}

// Motors operational?
bool MotorStatus() {
  return GetStatus(MODULE_MOTOR) && GetStatus(MODULE_PWR_MOTOR);
}
