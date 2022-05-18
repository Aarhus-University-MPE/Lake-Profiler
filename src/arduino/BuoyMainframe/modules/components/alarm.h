/*/
  Clock Functionalities
*/

#pragma once
#include "../setup/modules.h"

#define MAX_ALARM_FREQUENCY 24
#define MIN_ALARM_FREQUENCY 1

#define MAX_ALARM_START     23
#define MIN_ALARM_START     0

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

int GetAlarmFrequency() {
  alarmFrequency = EEPROM_READ_INT(MEMADDR_ALARM_FREQUENCY);
  alarmFrequency = min(MAX_ALARM_FREQUENCY, max(MIN_ALARM_FREQUENCY, alarmFrequency));

  return alarmFrequency;
}

int GetAlarmStartHour() {
  alarmStartHour = EEPROM_READ_INT(MEMADDR_ALARM_START);
  alarmStartHour = min(MAX_ALARM_START, max(MIN_ALARM_START, alarmStartHour));

  return alarmStartHour;
}

void InitializeAlarm() {
  SetAlarmFrequency(GetAlarmFrequency());
  SetAlarmStartHour(GetAlarmStartHour());
  SetAlarm();
}

// set alarm based on input hour
void SetAlarm(byte hour, uint8_t src) {
  if (!GetStatus(MODULE_CLOCK)) {
    DEBUG_PRINTLN(F("Clock Error"));
    return;
  }
  RTCC.setSec(src, 0x00);
  RTCC.setMin(src, 0x00);
  RTCC.setHour(src, HourToHex(HexToHour(hour) - 1));  // Start 1 hour before for warmup
  RTCC.setDay(src, 0x01);
  RTCC.setDate(src, 0x01);
  RTCC.setMonth(src, 0x01);

  EnableAlarm(src);

  // print alarm 1
  if (src == RTCC_ALM0) {
    DEBUG_PRINT(F("Alarm 0 is set to : "));
  } else {
    DEBUG_PRINT(F("Alarm 1 is set to : "));
  }
  printTime(src);
}

// set alarm 0 based on input hour
void SetAlarm(byte hour) {
  SetAlarm(hour, RTCC_ALM0);
}

// Set Alarm1 to 1 hour from now
uint8_t SetAlarmHourFromNow() {
  uint8_t hourNow = RTCC.getHour(RTCC_RTCC);
  SetAlarm(HourToHex(HexToHour(hourNow) + 1), RTCC_ALM1);
  return (HexToHour(hourNow) + 1);
}

void SetAlarmFrequency(uint8_t frequency) {
  alarmFrequency = frequency;
  EEPROM_WRITE_INT(MEMADDR_ALARM_FREQUENCY, alarmFrequency);

  // DEBUG_PRINT(F("Alarm Frequency Set: "));
  // DEBUG_PRINTLN(alarmFrequency);

  UpdateAlarmTimings();
}

void SetAlarmStartHour(uint8_t startHourHex) {
  alarmStartHour = startHourHex;
  EEPROM_WRITE_INT(MEMADDR_ALARM_START, alarmStartHour);

  // DEBUG_PRINT(F("Alarm Start Hour Set: "));
  // DEBUG_PRINT2(alarmStartHour, HEX);
  // DEBUG_PRINTLN();

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
  RTCC.disableAlarm(alarm);
  RTCC.enableAlarm(alarm, RTCC_ALMC1);
}

bool CheckAlarm() {
  if (!RTCC.checkFlag(RTCC_ALM0)) {
    return false;
  }

  DEBUG_PRINTLN(F("Alarm 0 triggered!"));
  SetAlarm();
  return true;
}

bool AlarmStatus(uint8_t src) {
  switch (src) {
    case RTCC_ALM0:
      return CheckAlarm();
      break;
    case RTCC_ALM1:
      if (RTCC.checkFlag(RTCC_ALM1)) {
        DEBUG_PRINTLN(F("Alarm 1 triggered!"));
        RTCC.disableAlarm(RTCC_ALM1);
        return true;
      }
    default:
      break;
  }
}

uint8_t NextAlarm() {
  uint8_t hour   = RTCC.getHour(RTCC_RTCC);
  uint8_t minute = RTCC.getMin(RTCC_RTCC);

  // DEBUG_PRINT(F("Current time: "));
  // DEBUG_PRINT2(hour, HEX);
  // DEBUG_PRINT(F(":"));
  // DEBUG_PRINT2(minute, HEX);
  // DEBUG_PRINTLN();

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
