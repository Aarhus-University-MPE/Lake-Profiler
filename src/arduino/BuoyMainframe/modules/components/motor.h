/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once

#include "../setup/modules.h"

byte motorState;

// Initialize motors and turns on encoder power
bool InitializeMotors() {
  digitalWrite(PO_POWER_ENCODER, true);

  // TODO: motor initialization...

  return true;
}

// Disables motors and turns off encoder power
void TerminateMotors() {
  digitalWrite(PO_POWER_ENCODER, false);

  // Motor termination
  digitalWrite(PO_MOTOR_DOWN, false);
  digitalWrite(PO_MOTOR_UP, false);
}

// Todo: test
unsigned long lastMillisRead;
void MotorProcess() {
  if (millis() - lastMillisRead < BUTTON_DBOUNCE_TIME) {
    return;
  }
  // TODO: DEBUG_PRINTLN(F("Reading... "));

  lastMillisRead = millis();
  if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
    MotorMove(MOTOR_DIR_UP);
  } else if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    MotorMove(MOTOR_DIR_DOWN);
  }
}

// move motors
void MotorMove(byte dir) {
  if (dir == motorState) {
    return;
  }

  if (motorState == MOTOR_DIR_UP || motorState == MOTOR_DIR_DOWN) {
    digitalWrite(PO_MOTOR_DOWN, false);
    digitalWrite(PO_MOTOR_UP, false);
    DEBUG_PRINTLN(F("Moving Halt"));
    motorState = MOTOR_DIR_HALT;
    delay(500);  // TODO: System halt
    return;
  }

  motorState = dir;
  switch (dir) {
    case MOTOR_DIR_UP:
      digitalWrite(PO_MOTOR_DOWN, false);
      digitalWrite(PO_MOTOR_UP, true);
      DEBUG_PRINTLN(F("Moving Up"));
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_DOWN:
      digitalWrite(PO_MOTOR_DOWN, true);
      digitalWrite(PO_MOTOR_UP, false);
      DEBUG_PRINTLN(F("Moving Down"));
      digitalWrite(LED_BUILTIN, true);
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

bool MotorPositionReached(uint8_t dir) {
  switch (dir) {
    case MOTOR_DIR_UP:
      return GetEncoderRotations() <= MOTOR_POS_BOT break;
      break;
    default:
      break;
  }
}

// Measure motor stall
bool MotorStall() {
  DEBUG_PRINTLN(F("Motor Stall!"));
  return false;
}

// // motor currently running?
// bool GetMotorState() {
//   return motorState;
// }

// Motors operational?
bool MotorStatus() {
  return GetStatus(MODULE_MOTOR) && GetStatus(MODULE_PWR_MOTOR);
}
