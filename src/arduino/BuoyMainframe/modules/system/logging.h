#pragma once
#include "../setup/modules.h"

bool datalogActive = false;

bool DataLogStart() {
  // Create logging files (.csv and .log)
  if (!InitializeLoggingFiles()) {
    DEBUG_PRINTLN(F("Initialization Failed - SD Error"));
    return false;
  }
  return true;
}

void DataLogStop() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Stopping Data Log"));
  DEBUG_PRINTLINE();

  AppendToLog(F("Data logging Stopped"), true);
  TimeStampLog();
  datalogActive = false;
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
  hourChar[3] = '\0';

  AppendToLog(hourChar);
  AppendToLog(F(":00"), true);
}

int GetWarmupTime() {
  return EEPROM_READ_INT(MEMADDR_SYSTEM_WARMUP);
}

void SetWarmupTime(int warmupTime) {
  EEPROM_WRITE_INT(MEMADDR_SYSTEM_WARMUP, warmupTime);
}

void TimeStampLog() {
  AppendToLog(F("Timestamp: "));
  AppendToLog((String)now(), true);
}

// Enable datalogging
void DataLogActivate() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Logging active!"));
  DEBUG_PRINTLINE();
  datalogActive = true;
}

bool DataLogActive() {
  return datalogActive;
}