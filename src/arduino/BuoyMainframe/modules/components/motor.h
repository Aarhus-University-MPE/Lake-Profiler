/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once

#include "../setup/modules.h"

byte motorState;
byte encoderMode;
unsigned long lastMillisRead, setStateStart;

// Initialize motors and turns on encoder power
bool InitializeMotors() {
  // Initialize outputs
  digitalWrite(PO_MOTOR_DOWN, false);
  digitalWrite(PO_MOTOR_UP, false);

  GetEncoderMode();

  EEPROMGetMotorPos();
  EEPROMGetTimeMotorPos();
  return true;
}

// Disables motors and turns off encoder power
void TerminateMotors() {
  // Save current position to EEPROM
  EEPROMSetMotorPos();
  EEPROMSetTimeMotorPos();

  MotorMove(MOTOR_DIR_HALT);

  // Motor termination
  digitalWrite(PO_MOTOR_DOWN, false);
  digitalWrite(PO_MOTOR_UP, false);
}

// Returns current motor State
byte MotorState() {
  return motorState;
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
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Setting Top Position"));
      DEBUG_PRINTLINE();
      SetEncoderTop();
      SetTimeEncoderTop();
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

  // Print current encoder position
  // DEBUG_PRINTLN(F("Current Position: "));
  // EncoderPrintPos();
  // TimeEncoderPrintPos();

  // Process direction command
  switch (dir) {
    case MOTOR_DIR_UP:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      TimeEncoderPrintPos(dir);

      TimeEncoderStart(dir);
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      TimeEncoderPrintPos(dir);

      TimeEncoderStart(dir);
      digitalWrite(PO_MOTOR_DOWN, true);
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_SERVICE:
      DEBUG_PRINT(F("Moving towards: "));
      EncoderPrintPos(dir);
      TimeEncoderPrintPos(dir);

      TimeEncoderStart(dir);
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_HALT:
      TimeEncoderUpdate();
      digitalWrite(PO_MOTOR_UP, false);
      digitalWrite(PO_MOTOR_DOWN, false);
      // DEBUG_PRINTLN(F("Moving Halt"));
      digitalWrite(LED_BUILTIN, false);
      break;
    default:
      break;
  }

  // Update current motor State
  motorState = dir;
}

unsigned long millisPrintMotorPos = 0;

// Returns true if endposition reached for specified direction
bool MotorPositionReached(uint8_t dir) {
  if (millis() - millisPrintMotorPos > 5000) {
    millisPrintMotorPos = millis();
    // EncoderPrintPos();
    TimeEncoderPrintPos();
    TimeEncoderPrintTarget(dir);
  }

  switch (encoderMode) {
    case ENCODERMODE_SICK:
      return MotorPositionReachedEncoder(dir);
      break;
    case ENCODERMODE_TIME:
      return MotorPositionReachedTimeEncoder(dir);
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

int GetEncoderMode() {
  encoderMode = EEPROM_READ_INT(MEMADDR_ENCODER_MODE);
  return encoderMode;
}