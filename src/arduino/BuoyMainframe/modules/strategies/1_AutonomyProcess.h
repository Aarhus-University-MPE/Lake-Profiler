/*  Autonomous Process

    Primary processes for autonomous modes
*/
#pragma once
#include "../setup/modules.h"

uint8_t autonomyState                 = 0;
unsigned long lastMillisAutonomyState = 0;
unsigned long millisAutonomyStart     = 0;

// Primary Autonomy Process for data logging
void AutonomyProcess() {
  // Check for manual override, will start logging if both buttons are pressed for 2 sec
  ButtonOverride();

  // Autonomy state machine cycle
  AutonomyState();

  // Receive data from canister
  recvWithStartEndMarkersCanister();
}

// Autonomy logging state machine
void AutonomyState() {
  switch (autonomyState) {
    // Idle, await preset alarm
    case 0:
      if (AlarmStatus(RTCC_ALM0)) autonomyState++;
      break;
    // Alarm triggered start logging process
    case 1:
      if (AutonomyStartLog()) autonomyState++;
      break;
    // Check Battery Levels
    case 2:
      if (AutonomyPowerCheck()) autonomyState++;
      break;
    // Power Levels sufficient, power up canister
    case 3:
      AutonomyStartCanister();
      autonomyState++;
      break;
    // Await handshake from canister
    case 4:
      if (AutonomyAwaitHandshake()) autonomyState++;
      break;
    // Handshake received, send start command to canister
    case 5:
      CanisterLogStart();
      millisAutonomyStart = millis();
      autonomyState++;
      break;
    // Await acknowledge from canister, confirming successful start
    case 6:
      if (AutonomyAwaitAcknowledge()) autonomyState++;
      break;
    // Acknowledge received, set alarm to 1 hr from now (CH4 warmup period)
    case 7:
      DataLogInitialized();
      autonomyState++;
      break;
    // Await alarm signalling warmup period over
    case 8:
      if (AlarmStatus(RTCC_ALM1)) autonomyState++;
      break;
    // Alarm triggered start moving motor up
    case 9:
      MotorMove(MOTOR_DIR_UP);
      autonomyState++;
      break;
    // Await top position reached
    case 10:
      if (MotorPositionReached(MOTOR_DIR_UP)) autonomyState++;
      break;
    // Top position reached, turn off canister and move to bottom position
    case 11:
      DataLogStop();
      MotorMove(MOTOR_DIR_DOWN);
      autonomyState++;
      break;
    // Await bottom position reached
    case 12:
      if (MotorPositionReached(MOTOR_DIR_DOWN)) autonomyState++;
      break;
    case 13:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Bottom position reached, system idle until next logging"));
      DEBUG_PRINTLN(F("Full log complete"));
      DEBUG_PRINTLINE();
      AppendToLog(F("Bottom position reached"), true);
      AppendToLog(F("Full log complete"), true);
      break;
    default:
      autonomyState = 0;
      break;
  }
}

// Start new data log
bool AutonomyStartLog() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Data Log"));
  DEBUG_PRINTLINE();
  if (DataLogStart()) {
    millisAutonomyStart = millis();
    DEBUG_PRINTLN(F("Awaiting Handshake... "));
    return true;
  } else {
    AutonomyStopLog();
    return false;
  }
}

// Stopping current data log
void AutonomyStopLog() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Stopping Data Log"));
  DEBUG_PRINTLINE();

  DataLogStop();
  autonomyState = 0;
}

// Check battery level
bool AutonomyPowerCheck() {
  DEBUG_PRINT(F("Battery Level: "));
  DEBUG_PRINT(BatteryLevel());
  AppendToLog(F("Battery Level: "));
  AppendToLog((String)BatteryLevel());
  if (BatteryStatus()) {
    AppendToLog(F(" \% - Sufficient"), true);
    DEBUG_PRINTLN(F(" \% - Sufficient"));
    return true;
  } else {
    AppendToLog(F(" % - Insufficient"), true);
    DEBUG_PRINTLN(F(" % - Insufficient"));
    AutonomyStopLog();
    return false;
  }
}

// Start Canister
void AutonomyStartCanister() {
  ModuleEnable(MODULE_PWR_CANISTER);
  ModuleEnable(MODULE_COMM_CANISTER);

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Awaiting Handshake... "));
  AppendToLog(F("Awaiting Handshake... "), false);
  DEBUG_PRINTLINE();
}

// Await handshake, timeout if too long
bool AutonomyAwaitHandshake() {
  // Check timeout
  if (millis() - millisAutonomyStart > LOGGING_START_TIMEOUT) {
    DEBUG_PRINTLN(F("Acknowledge timeout Error"));
    AutonomyStopLog();
    return false;
  }

  return AcknowledgeReceived();
}

// Await Acknowledge, timeout if too long
bool AutonomyAwaitAcknowledge() {
  // Check timeout
  if (millis() - millisAutonomyStart > LOGGING_START_TIMEOUT) {
    DEBUG_PRINTLN(F("Handshake timeout Error"));
    AutonomyStopLog();
    return false;
  }

  return HandshakeReceived();
}

// Manual Override of Autonomy Process, start/stop logging
void ButtonOverride() {
  // Check if one of the buttons are not pressed
  if (digitalRead(PI_BUTTON_MOTOR_DOWN) || digitalRead(PI_BUTTON_MOTOR_UP)) return;

  // Wait hold period
  delay(2000);

  // Check if one of the buttons are have been released
  if (digitalRead(PI_BUTTON_MOTOR_DOWN) || digitalRead(PI_BUTTON_MOTOR_UP)) return;

  // If currently Logging, Stop
  if (autonomyState != 0) {
    AutonomyStopLog();
  }
  // else (currently inactive) start logging
  else {
    autonomyState = 1;
  }
}

// Move motor up until at end pos, then return true
void AutonomyMotorProcess() {
}