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
    // Idle, await Warmup alarm
    // TODO: incoorporate minutes into alarm (once alarm 1 triggers (hour) set alarm to minutes and await 2nd trigger)
    case 0:
      if (AlarmStatus(RTCC_ALM0)) {
        autonomyState++;
        DEBUG_PRINTLN(F("Alarm case 0"));
      }
      break;
    // Alarm triggered start logging process
    case 1:
      if (AutonomyStartLog()) {
        autonomyState++;
      } else {
        autonomyState = 0;
        SetAlarm();
      }
      break;
    // Check Battery Levels
    case 2:
      if (AutonomyPowerCheck()) {
        autonomyState++;
      } else {
        autonomyState = 0;
        SetAlarm();
      }
      break;
    // Power Levels sufficient, power up canister
    case 3:
      AutonomyStartCanister();
      autonomyState++;
      break;
    // Await status from canister
    case 4:
      if (AutonomyAwaitHandshake()) autonomyState++;  // TODO: rework to await a message
      break;
    // Status received, set alarm to 1 hr from now (CH4 warmup period)
    case 5:
      DataLogInitialized();
      autonomyState++;
      break;
    // Await alarm signalling warmup period over
    case 6:
      if (AlarmStatus(RTCC_ALM1)) autonomyState++;
      break;
    // Alarm triggered start moving motor up
    case 7:
      MotorMove(MOTOR_DIR_UP);
      autonomyState++;
      break;
    // Await top position reached
    case 8:
      if (MotorPositionReached()) {
        MotorMove(MOTOR_DIR_HALT);
        autonomyState++;
      }
      break;
    // Top position reached, turn off canister and move to bottom position
    case 9:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Top Position Reached, powering off canister"));
      DEBUG_PRINTLINE();
      AppendToLog(F("Top Position Reached, powering off canister"));

      ModuleDisable(MODULE_PWR_CANISTER);
      ModuleDisable(MODULE_COMM_CANISTER);

      MotorMove(MOTOR_DIR_DOWN);
      autonomyState++;
      break;
    // Await bottom position reached
    case 10:
      if (MotorPositionReached()) {
        MotorMove(MOTOR_DIR_HALT);
        autonomyState++;
      }
      break;
    // Bottom position reached, turn off motor and stop data logging
    case 11:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Bottom position reached"));
      DEBUG_PRINTLINE();
      AppendToLog(F("Bottom position reached"), true);
      AppendToLog(F("Full log complete"), true);
      AutonomyStopLog();  // TODO: Remove?
      autonomyState++;
      break;
    // Start LoRa Broadcast
    case 12:
      LoRaBroadcastBegin();
      autonomyState++;
      break;
    // Start Broadcasting log file
    case 13:
      if (LoRaBroadcastLog()) autonomyState++;
      break;
    // Log file broadcast, start Broadcasting data file
    case 14:
      if (LoRaBroadcastData()) autonomyState++;
      break;
    case 15:
      DEBUG_PRINTLINE();
      DEBUG_PRINT(F("Logging Sample Complete, Waiting for next timestamp - "));
      PrintAlarmTime();
      DEBUG_PRINTLINE();
      autonomyState = 0;
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
    return true;
  } else {
    AutonomyStopLog();
    return false;
  }
}

// Stopping current autonomous log
void AutonomyStopLog() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Stopping Autonomous Log"));
  DEBUG_PRINTLINE();

  DataLogStop();

  // autonomyState = 0; TODO: ??
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
    DEBUG_PRINTLINE();
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
    DEBUG_PRINTLN(F("Handshake timeout Error"));
    AutonomyStopLog();
    return false;
  }

  return HandshakeReceived();
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

  // If currently inactive start logging (Waiting for Alarm0 or Alarm1)
  if (autonomyState == 6 || autonomyState == 0) {
    autonomyState++;
  }
  // If currently logging, stop
  else {
    AutonomyStopLog();
  }
}

bool LoggingActive() {
  return autonomyState > 0;
}