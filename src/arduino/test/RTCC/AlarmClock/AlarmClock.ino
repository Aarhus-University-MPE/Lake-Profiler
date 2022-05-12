/*
  Simple Alarm Test Sketch
  Pmod RTCC
*/
#include <RTCCI2C.h>

#include "TimeLib.h"

tmElements_t te;  // Time elements structure
time_t unixTime;  // a time stamp

uint8_t alarmFrequency = 0;
uint8_t alarmStartHour = 0;
uint8_t alarm[24];

RTCCI2C RTCC;
// int year, mon, date, day, hour, minute, sec;
byte alarmState;
const byte maxAlarmStates     = 3;
unsigned long lastMillisAlarm = 0;

void setup() {
  Serial.begin(115200);

  if (!InitializeRTC()) {
    Serial.println(F("Clock Initialization: Failed"));
    while (1)
      ;
  }
  SetClockYear(0x22);
  SetClockMonth(0x05);
  SetClockDate(0x12);
  SetClockHour(0x11);
  SetClockMin(0x59);
  SetAlarmFrequency(12);
  SetAlarmStartHour(6);
  SetAlarm();
}

void loop() {
  if (millis() - lastMillisAlarm < 5000) {
    return;
  }
  lastMillisAlarm = millis();
  RTCPrint();
  // if (RTCC.checkFlag(RTCC_ALM0)) {
  //   Serial.println(F("Alarm triggered!"));
  //   UpdateAlarm();
  // }
}

uint8_t HourToHex(uint8_t hour) {
  if (hour < 10) {
    return hour;
  } else if (hour < 20) {
    return hour + 6;
  } else if (hour < 24) {
    return hour + 12;
  } else {
    return HourToHex(hour - 24);
  }
}

bool InitializeRTC() {
  RTCC.begin();

  // print the power-fail time-stamp
  // Serial.print("Lost Power at: ");
  // printTime(RTCC_PWRD);
  // Serial.print("Power was back at: ");
  // printTime(RTCC_PWRU);

  // enable back up battery
  RTCC.enableVbat();

  // Update Arduino Clock to match RTCC
  UpdateUnixTime(RTCC_RTCC);
  alarmState = 0;
  // RTCPrint();
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

void SetClockSec(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setSec(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockMin(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setMin(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockHour(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setHour(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockDay(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setDay(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockDate(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setDate(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockMonth(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setMonth(RTCC_RTCC, hexValue);
  RTCC.startClock();
}

void SetClockYear(uint8_t hexValue) {
  RTCC.stopClock();
  RTCC.setYear(hexValue);
  RTCC.startClock();
}

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

void TerminateRTC() {
  RTCC.stopClock();
}

bool RTCStatus() {
  // return GetStatus(MODULE_CLOCK);
  return true;  // TODO: Remove
}

// Configs alarm
void SetAlarm() {
  if (!RTCStatus()) {
    return;
  }
  SetAlarm(NextAlarm());
}

// set alarm 0 based on input hour
void SetAlarm(byte hour) {
  if (!RTCStatus()) {
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
  Serial.print(F("Alarm 0 set: "));
  printTime(RTCC_ALM0);
}

uint8_t NextAlarm() {
  uint8_t hour   = RTCC.getHour(RTCC_RTCC);
  uint8_t minute = RTCC.getMin(RTCC_RTCC);

  Serial.print(F("Current time: "));
  Serial.print(hour, HEX);
  Serial.print(F(":"));
  Serial.println(minute, HEX);

  int8_t hourDifference = -1;
  uint8_t i             = 0;

  // Serial.print(F("Current hour: "));
  // Serial.print(hour, HEX);
  hourDifference = alarm[i] - hour;

  while (hourDifference <= 0 && i < alarmFrequency) {
    i++;
    hourDifference = alarm[i] - hour;
  }
  // Serial.print(F(", next closest alarm: "));
  // Serial.println(alarm[i], HEX);
  return alarm[i];
}

void SetAlarmState(uint8_t state) {
  alarmState = state;
}

void SetAlarmFrequency(uint8_t frequency) {
  alarmFrequency = frequency;
  UpdateAlarmTimings();
}

void SetAlarmStartHour(uint8_t startHour) {
  alarmStartHour = startHour;
  UpdateAlarmTimings();
}

void UpdateAlarmTimings() {
  uint8_t alarmIntervals = 24 / alarmFrequency;  // Convert to hex?

  for (uint8_t i = 0; i < alarmFrequency; i++) {
    alarm[i] = HourToHex(alarmStartHour + alarmIntervals * i);
  }

  // Serial.println(F("Alarms timings: "));
  // for (uint8_t i = 0; i < alarmFrequency; i++) {
  //   Serial.print(alarm[i], HEX);
  //   Serial.print(F(", "));
  // }
  // Serial.println();
}

void UpdateAlarm() {
  if (alarmState > alarmFrequency - 1) {
    alarmState = 0;
  } else {
    alarmState += 1;
  }

  SetAlarm();
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

// Update clock to current unixTime
void UpdateUnixTime(uint8_t src) {
  // Get time from RTCC
  te.Second = RTCC.getSec(src);
  te.Minute = RTCC.getMin(src);
  te.Hour   = RTCC.getHour(src);
  te.Day    = RTCC.getDate(src);
  te.Month  = RTCC.getMonth(src);
  te.Year   = RTCC.getYear() - 1970;

  // Convert to unixtime
  unixTime = makeTime(te);

  // Set MCU clock to RTCC time, now() returns correct time
  setTime(unixTime);
}

void RTCPrint() {
  // print current time
  Serial.print(F("Current time: "));
  printTime(RTCC_RTCC);

  // check if alarm 0 is triggered
  if (RTCC.checkFlag(RTCC_ALM0)) {
    // alarm 0 has been triggered
    Serial.println(F("ALM0!!!"));
    // disable alarm 0, alarm 0 will not be triggered anymore
    RTCC.disableAlarm(RTCC_ALM0);
    UpdateAlarm();
  }
}

void printTime(uint8_t src) {
  // get all paratmeters of the src
  // UpdateUnixTime(src);

  // Serial.println(unixTime);

  // print all parameter of the src
  if (src == RTCC_RTCC) {
    Serial.print(RTCC.getMonth(src), HEX);
    Serial.print(F("/"));
    Serial.print(RTCC.getDate(src), HEX);
    // year is only available for the RTCC
    if (src == RTCC_RTCC) {
      Serial.print(F("/"));
      Serial.print(RTCC.getYear(), HEX);
    }
    Serial.print(F(" - "));
  }
  Serial.print(RTCC.getHour(src), HEX);
  Serial.print(F(":"));
  Serial.print(RTCC.getMin(src), HEX);

  // second is not supported by the power fail registers
  if (src != RTCC_PWRD && src != RTCC_PWRU) {
    Serial.print(F(":"));
    Serial.print(RTCC.getSec(src), HEX);
  }

  Serial.println();
}

void GetTimeStamp(char* fileName) {
  char tempArray[3];

  // YY
  itoa(RTCC.getYear(), tempArray, 10);
  fileName[0] = tempArray[0];
  fileName[1] = tempArray[1];

  // MM
  itoa(RTCC.getMonth(RTCC_RTCC), tempArray, 10);
  fileName[2] = tempArray[0];
  fileName[3] = tempArray[1];

  // DD
  itoa(RTCC.getDate(RTCC_RTCC), tempArray, 10);
  fileName[4] = tempArray[0];
  fileName[5] = tempArray[1];

  // HH
  itoa(RTCC.getHour(RTCC_RTCC), tempArray, 10);
  fileName[6] = tempArray[0];
  fileName[7] = tempArray[1];
}
