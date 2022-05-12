/*/
  Clock Functionalities
*/

#pragma once
#include "../setup/modules.h"

uint8_t alarmFrequency = 0;
uint8_t alarmStartHour = 0;
uint8_t alarm[24];

void SetAlarmSec(uint8_t hexValue) {
  RTCC.setSec(RTCC_ALM0, hexValue);
}

void SetAlarmMin(uint8_t hexValue) {
  RTCC.setMin(RTCC_ALM0, hexValue);
}

void SetAlarmHour(uint8_t hexValue) {
  RTCC.setHour(RTCC_ALM0, hexValue);
}

void SetAlarmDay(uint8_t hexValue) {
  RTCC.setDay(RTCC_ALM0, hexValue);
}

void SetAlarmDate(uint8_t hexValue) {
  RTCC.setDate(RTCC_ALM0, hexValue);
}

void SetAlarmMonth(uint8_t hexValue) {
  RTCC.setMonth(RTCC_ALM0, hexValue);
}

// Configs alarm
void SetAlarm() {
  if (!RTCStatus()) {
    return;
  }
  SetAlarm(NextAlarm());
}

void InitializeAlarm() {
  SetAlarmFrequency(12);  // TODO: Add to EEPROM
  SetAlarmStartHour(6);
  SetAlarm();
}

// set alarm 0 based on input hour
void SetAlarm(byte hour) {
  if (!GetStatus(MODULE_CLOCK)) {
    DEBUG_PRINTLN(F("Clock Error"));
    return;
  }
  RTCC.setSec(RTCC_ALM0, 0x00);
  RTCC.setMin(RTCC_ALM0, 0x00);
  RTCC.setHour(RTCC_ALM0, hour);
  RTCC.setDay(RTCC_ALM0, 0x01);
  RTCC.setDate(RTCC_ALM0, 0x01);
  RTCC.setMonth(RTCC_ALM0, 0x01);

  EnableAlarm(RTCC_ALM0);

  // print alarm 1
  DEBUG_PRINT(F("Alarm 0 is set to : "));
  printTime(RTCC_ALM0);
}

void SetAlarmFrequency(uint8_t frequency) {
  alarmFrequency = frequency;
  UpdateAlarmTimings();
}

void SetAlarmStartHour(uint8_t startHourHex) {
  alarmStartHour = startHourHex;
  UpdateAlarmTimings();
}

void UpdateAlarmTimings() {
  uint8_t alarmIntervals = 24 / alarmFrequency;

  for (uint8_t i = 0; i < alarmFrequency; i++) {
    alarm[i] = HourToHex(alarmStartHour + alarmIntervals * i);
  }
}

/*
  NONE                       - seconds match
  RTCC_ALMC0                 - minutes match
  RTCC_ALMC1                 - hours match
  RTCC_ALMC1 | ALMC0         - matches on day at midnight
  RTCC_ALMC2                 - date match
  RTCC_ALMC2 | ALMC1 | ALMC0 - seconds, minutes, hour, day, date, and month match
*/
void EnableAlarm(uint8_t alarm) {
  RTCC.enableAlarm(alarm, RTCC_ALMC1);
}

bool CheckAlarm() {
  if (RTCC.checkFlag(RTCC_ALM0)) {
    DEBUG_PRINTLN(F("Alarm triggered!"));
    SetAlarm();
  }
}

uint8_t NextAlarm() {
  uint8_t hour   = RTCC.getHour(RTCC_RTCC);
  uint8_t minute = RTCC.getMin(RTCC_RTCC);

  DEBUG_PRINT(F("Current time: "));
  DEBUG_PRINT2(hour, HEX);
  DEBUG_PRINT(F(":"));
  DEBUG_PRINT2(minute, HEX);
  DEBUG_PRINTLN();

  int8_t hourDifference = -1;
  uint8_t i             = 0;

  hourDifference = alarm[i] - hour;

  while (hourDifference <= 0 && i < alarmFrequency) {
    i++;
    hourDifference = alarm[i] - hour;
  }
  // Serial.print(F(", next closest alarm: "));
  // Serial.println(alarm[i], HEX);
  return alarm[i];
}
