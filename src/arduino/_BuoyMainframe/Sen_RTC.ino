/*
    Real Time Clock handles timing using a Pmod RTCC (https://digilent.com/reference/pmod/pmodrtcc/start?redirect=1)

    Primary library: https://digilent.com/reference/_media/reference/pmod/pmodrtcc/rtcci2c.zip
    - Modified to latest Wire.h convention (Wire.receive() -> Wire.read() & Wire.send(xx) -> Wire.write(xx))

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk


*/

#include <RTCCI2C.h>

RTCCI2C RTCC;
int year, mon, date, day, hour, minute, sec;

bool InitializeRTC() {
  RTCC.begin();

  // print the power-fail time-stamp
  Serial.print("Lost Power at: ");
  printTime(RTCC_PWRD);
  Serial.print("Power was back at: ");
  printTime(RTCC_PWRU);

  // set the real time clock
  RTCC.stopClock();
  RTCC.setSec(RTCC_RTCC, 0x30);
  RTCC.setMin(RTCC_RTCC, 0x39);
  RTCC.setHour(RTCC_RTCC, 0x13);
  RTCC.setDay(RTCC_RTCC, 0x05);
  RTCC.setDate(RTCC_RTCC, 0x04);
  RTCC.setMonth(RTCC_RTCC, 0x08);
  RTCC.setYear(0x22);
  RTCC.startClock();

  // set alarm 0
  RTCC.setSec(RTCC_ALM0, 0x00);
  RTCC.setMin(RTCC_ALM0, 0x40);
  RTCC.setHour(RTCC_ALM0, 0x13);
  RTCC.setDay(RTCC_ALM0, 0x05);
  RTCC.setDate(RTCC_ALM0, 0x08);
  RTCC.setMonth(RTCC_ALM0, 0x04);

  // set alarm 1
  RTCC.setSec(RTCC_ALM1, 0x00);
  RTCC.setMin(RTCC_ALM1, 0x41);
  RTCC.setHour(RTCC_ALM1, 0x13);
  RTCC.setDay(RTCC_ALM1, 0x05);
  RTCC.setDate(RTCC_ALM1, 0x08);
  RTCC.setMonth(RTCC_ALM1, 0x04);

  // print current time
  Serial.print("Current time is: ");
  printTime(RTCC_RTCC);
  // print alarm 0
  Serial.print("Alarm 0 is set to : ");
  printTime(RTCC_ALM0);
  // print alarm 1
  Serial.print("Alarm 1 is set to : ");
  printTime(RTCC_ALM1);

  // enables alarm 0
  RTCC.enableAlarm(RTCC_ALM0, RTCC_ALMC2 | RTCC_ALMC1);

  RTCC.enableAlarm(RTCC_ALM1, RTCC_ALMC2 | RTCC_ALMC1);

  // enable back up battery
  // RTCC.enableVbat();

  return true;
}

bool TerminateRTC() {
  return true;
}

bool RTCStatus() {
  return GetStatus(MODULE_CLOCK);
}

// Configs alarm
void SetAlarm(uint8_t alarm) {
  RTCC.setSec(alarm, 0x00);
  RTCC.setMin(alarm, 0x40);
  RTCC.setHour(alarm, 0x13);
  RTCC.setDay(alarm, 0x05);
  RTCC.setDate(alarm, 0x08);
  RTCC.setMonth(alarm, 0x04);
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

void RTCPrint() {
  // print current time
  Serial.print("Current time is : ");
  printTime(RTCC_RTCC);

  // check if alarm 0 is triggered
  if (RTCC.checkFlag(RTCC_ALM0)) {
    // alarm 0 has been triggered
    Serial.println("ALM0!!!");
    // disable alarm 0, alarm 0 will not be triggered anymore
    RTCC.disableAlarm(RTCC_ALM0);
  }

  // check if alarm 1 is triggered
  if (RTCC.checkFlag(RTCC_ALM1)) {
    // alarm 1 has been triggered
    Serial.println("ALM1!!!");
    // turn off alarm 1, alarm 1 will still be triggered on next match
    RTCC.alarmOff(RTCC_ALM1);
  }
}

void printTime(uint8_t src) {
  // get all paratmeters of the src
  sec    = RTCC.getSec(src);
  minute = RTCC.getMin(src);
  hour   = RTCC.getHour(src);
  day    = RTCC.getDay(src);
  date   = RTCC.getDate(src);
  mon    = RTCC.getMonth(src);
  year   = RTCC.getYear();

  // print all parameter of the src
  Serial.print(mon, HEX);
  Serial.print("/");
  Serial.print(date, HEX);
  // year is only available for the RTCC
  if (src == RTCC_RTCC) {
    Serial.print("/");
    Serial.print(year, HEX);
  }
  Serial.print("  day");
  Serial.print(day, HEX);
  Serial.print("  ");
  Serial.print(hour, HEX);
  Serial.print(":");
  Serial.print(minute, HEX);

  // second is not supported by the power fail registers
  if (src != RTCC_PWRD && src != RTCC_PWRU) {
    Serial.print(":");
    Serial.print(sec, HEX);
  }

  Serial.println();
}
