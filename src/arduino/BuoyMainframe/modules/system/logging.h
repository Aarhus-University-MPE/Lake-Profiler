#pragma once
#include "../setup/modules.h"

bool DataLogStart() {
  // Create logging files (.csv and .log)
  if (!InitializeLoggingFiles()) {
    DEBUG_PRINTLN(F("Initialization Failed - SD Error"));
    AutonomyStopLog();
    return false;
  }

  // Start Canister
  ModuleEnable(MODULE_PWR_CANISTER);
  ModuleEnable(MODULE_COMM_CANISTER);

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Awaiting Handshake... "));
  AppendToLog(F("Awaiting Handshake... "), false);
  return true;
}

void DataLogStop() {
  ModuleDisable(MODULE_PWR_CANISTER);
  ModuleDisable(MODULE_COMM_CANISTER);
  AppendToLog(F("Log Complete"), true);
  // TODO: Write timestamp (UNIX)
}

void DataLogInitialized() {
  DEBUG_PRINTLN(F("Setting Alarm to 1 hour from now"));
  AppendToLog(F("Setting Alarm to 1 hour from now - "));
  uint8_t alarmHour = SetAlarmHourFromNow();

  char tempArray[3];
  char hourChar[3];
  itoa(alarmHour, tempArray, 10);
  if (alarmHour < 10) {
    hourChar[0] = '0';
    hourChar[1] = tempArray[0];
  } else {
    hourChar[0] = tempArray[0];
    hourChar[1] = tempArray[1];
  }
  AppendToLog(hourChar);
  AppendToLog(F(":00"));

  DEBUG_PRINTLN(F("Alarm Set: "));
  DEBUG_PRINTLN(hourChar);
  DEBUG_PRINTLN(F(":00"));
}
