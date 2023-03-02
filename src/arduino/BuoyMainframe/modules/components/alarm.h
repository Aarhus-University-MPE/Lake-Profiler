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
  SetAlarmStartHour(GetAlarmStartHour());
  SetAlarmFrequency(GetAlarmFrequency());
  SetAlarm();
}

// set alarm based on input hour
void SetAlarm(byte hourHex, uint8_t src) {
  if (!GetStatus(MODULE_CLOCK)) {
    DEBUG_PRINTLN(F("Clock Error"));
    return;
  }

  if (hourHex == 0x24) hourHex = 0x00;

  RTCC.setSec(src, 0x00);
  RTCC.setMin(src, 0x00);
  RTCC.setHour(src, hourHex);  // Start 1 hour before for warmup
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
  // DEBUG_PRINT(F("Current hour: "));
  // DEBUG_PRINT2(hourNow, HEX);
  // DEBUG_PRINTLN();

  // DEBUG_PRINT(F("1 hour from now: "));
  // DEBUG_PRINT2(HourToHex(HexToHour(hourNow) + 1), HEX);
  // DEBUG_PRINTLN();

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

void SetAlarmStartHour(uint8_t startHour) {
  alarmStartHour = startHour;
  EEPROM_WRITE_INT(MEMADDR_ALARM_START, alarmStartHour);

  // DEBUG_PRINT(F("Alarm Start Hour Set: "));
  // DEBUG_PRINT2(alarmStartHour, HEX);
  // DEBUG_PRINTLN();

  UpdateAlarmTimings();
}

void UpdateAlarmTimings() {
  uint8_t alarmIntervals = 24 / alarmFrequency;
  DEBUG_PRINT(F("Alarm Hours: "));
  for (uint8_t i = 0; i < alarmFrequency; i++) {
    alarm[i] = HourToHex(alarmStartHour + alarmIntervals * i);
    DEBUG_PRINT2(alarm[i], HEX);
    DEBUG_PRINT(F(":00 \t"));
  }
  DEBUG_PRINTLN();
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

void PrintAlarmTime() {
  DEBUG_PRINT2(RTCC.getHour(RTCC_ALM0), HEX);
  DEBUG_PRINTLN(F(":00"));
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
  return false;
}

uint8_t NextAlarm() {
  uint8_t hour = RTCC.getHour(RTCC_RTCC);

  hour = HexToHour(hour);

  int8_t hourDifference = -1;
  int i                 = 0;

  hourDifference = HexToHour(alarm[i]) - hour;
  if (alarmFrequency == 1 && hourDifference < 0) {
    // TODO: Set for next day
  }

  while (hourDifference < GetWarmupTime() + 1 && i < alarmFrequency - 1) {
    i++;
    hourDifference = HexToHour(alarm[i]) - hour;
  }

  if (hourDifference < GetWarmupTime() + 1) {
    i = 0;
  }

  return HourToHex(HexToHour(alarm[i]) - GetWarmupTime());
}
