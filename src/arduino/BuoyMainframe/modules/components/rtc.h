#pragma once
#include "../setup/modules.h"

/*
    Real Time Clock handles timing using a Pmod RTCC (https://digilent.com/reference/pmod/pmodrtcc/start?redirect=1)

    Primary library: https://digilent.com/reference/_media/reference/pmod/pmodrtcc/rtcci2c.zip
    - Modified to latest Wire.h convention (Wire.receive() -> Wire.read() & Wire.send(xx) -> Wire.write(xx))
    Secondary library: https://github.com/PaulStoffregen/Time

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk


*/

#include <RTCCI2C.h>

#include "TimeLib.h"

tmElements_t te;  // Time elements structure
time_t unixTime;  // a time stamp

RTCCI2C RTCC;
// int year, mon, date, day, hour, minute, sec;
byte alarmState;
const byte maxAlarmStates = 2;

bool InitializeRTC() {
  RTCC.begin();

  // print the power-fail time-stamp
  // DEBUG_PRINT("Lost Power at: ");
  // printTime(RTCC_PWRD);
  // DEBUG_PRINT("Power was back at: ");
  // printTime(RTCC_PWRU);

  // enable back up battery
  RTCC.enableVbat();

  // Update Arduino Clock to match RTCC
  UpdateClock();

  return RTCC.getYear() != 0;
}

void SetClock() {
  // set the real time clock
  RTCC.stopClock();
  RTCC.setSec(RTCC_RTCC, 0x00);
  RTCC.setMin(RTCC_RTCC, 0x00);
  RTCC.setHour(RTCC_RTCC, 0x12);
  RTCC.setDay(RTCC_RTCC, 0x05);
  RTCC.setDate(RTCC_RTCC, 0x06);
  RTCC.setMonth(RTCC_RTCC, 0x05);
  RTCC.setYear(0x22);
  RTCC.startClock();
}

void TerminateRTC() {
  RTCC.stopClock();
}

bool RTCStatus() {
  return GetStatus(MODULE_CLOCK);
}

// Configs alarm
void SetAlarm() {
  if (!GetStatus(MODULE_CLOCK)) {
    return;
  }
  alarmState = 0;

  // set alarm 0
  RTCC.setSec(RTCC_ALM0, 0x00);
  RTCC.setMin(RTCC_ALM0, 0x00);
  RTCC.setHour(RTCC_ALM0, 0x13);
  RTCC.setDay(RTCC_ALM0, 0x01);
  RTCC.setDate(RTCC_ALM0, 0x01);
  RTCC.setMonth(RTCC_ALM0, 0x01);

  // enables alarm 0
  RTCC.enableAlarm(RTCC_ALM0, RTCC_ALMC1);

  // print current time
  DEBUG_PRINT(F("Current time is: "));
  printTime(RTCC_RTCC);

  // print alarm 0
  DEBUG_PRINT(F("Alarm 0 is set to : "));
  printTime(RTCC_ALM0);
}

// set alarm 0 based on input hour
void SetAlarm(byte hour) {
  if (!GetStatus(MODULE_CLOCK)) {
    return;
  }
  RTCC.setSec(RTCC_ALM0, 0x00);
  RTCC.setMin(RTCC_ALM0, 0x00);
  RTCC.setHour(RTCC_ALM0, hour);
  RTCC.setDay(RTCC_ALM0, 0x01);
  RTCC.setDate(RTCC_ALM0, 0x01);
  RTCC.setMonth(RTCC_ALM0, 0x01);

  RTCC.enableAlarm(RTCC_ALM0, RTCC_ALMC1);

  // print alarm 1
  DEBUG_PRINT(F("Alarm 0 is set to : "));
  printTime(RTCC_ALM0);
}

void UpdateAlarm() {
  if (alarmState + 1 > maxAlarmStates) {
    alarmState = 0;
  } else {
    alarmState += 1;
  }

  switch (alarmState) {
    case 0:
      SetAlarm(0x13);
      break;
    case 1:
      SetAlarm(0x14);
      break;
    case 2:
      SetAlarm(0x15);
      break;
    default:
      break;
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
  RTCC.enableAlarm(alarm, RTCC_ALMC1 | RTCC_ALMC0);
}

// Update clock to current unixTime
void UpdateClock() {
  // Get time from RTCC
  te.Second = RTCC.getSec(RTCC_RTCC);
  te.Minute = RTCC.getMin(RTCC_RTCC);
  te.Hour   = RTCC.getHour(RTCC_RTCC);
  te.Day    = RTCC.getDay(RTCC_RTCC);
  te.Month  = RTCC.getMonth(RTCC_RTCC);
  te.Year   = RTCC.getYear() - 1970;

  // Convert to unixtime
  unixTime = makeTime(te);

  // Set MCU clock to RTCC time, now() returns correct time
  setTime(unixTime);
}

void RTCPrint() {
  // print current time
  DEBUG_PRINT(F("Current time is : "));
  printTime(RTCC_RTCC);

  // check if alarm 0 is triggered
  if (RTCC.checkFlag(RTCC_ALM0)) {
    // alarm 0 has been triggered
    DEBUG_PRINTLN(F("ALM0!!!"));
    // disable alarm 0, alarm 0 will not be triggered anymore
    RTCC.disableAlarm(RTCC_ALM0);
    UpdateAlarm();
  }
}

void printTime(uint8_t src) {
  // get all paratmeters of the src
  UpdateClock();

  DEBUG_PRINTLN(unixTime);

  // year   = RTCC.getYear();

  // // print all parameter of the src
  // DEBUG_PRINT2(mon, HEX);
  // DEBUG_PRINT(F("/"));
  // DEBUG_PRINT2(date, HEX);
  // year is only available for the RTCC
  // if (src == RTCC_RTCC) {
  //   DEBUG_PRINT(F("/"));
  //   // DEBUG_PRINT2(year, HEX);
  // }
  // DEBUG_PRINT(F("  day"));
  // DEBUG_PRINT2(day, HEX);
  // DEBUG_PRINT(F("  "));
  // DEBUG_PRINT2(hour, HEX);
  // DEBUG_PRINT(F(":"));
  // DEBUG_PRINT2(minute, HEX);

  // // second is not supported by the power fail registers
  // if (src != RTCC_PWRD && src != RTCC_PWRU) {
  //   DEBUG_PRINT(F(":"));
  //   DEBUG_PRINT2(sec, HEX);
  // }

  DEBUG_PRINTLN();
}
