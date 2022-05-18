#pragma once
#include "../setup/modules.h"

bool DataLogStart() {
  // Create logging files (.csv and .log)
  if (!InitializeLoggingFiles()) {
    DEBUG_PRINTLN(F("Initialization Failed - SD Error"));
    return false;
  }
  return true;
}

void DataLogStop() {
  ModuleDisable(MODULE_PWR_CANISTER);
  ModuleDisable(MODULE_COMM_CANISTER);
  AppendToLog(F("Log Complete"), true);
  AppendToLog(F("Timestamp: "));
  AppendToLog((String)now(), true);
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
