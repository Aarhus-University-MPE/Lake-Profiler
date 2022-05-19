/*/
  Clock Functionalities
*/

#pragma once
#include "../setup/modules.h"

tmElements_t te;  // Time elements structure
time_t unixTime;  // a time stamp

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

// Update clock to current unixTime
void UpdateUnixTime() {
  // Get time from RTCC
  te.Second = HexToHour(RTCC.getSec(RTCC_RTCC));
  te.Minute = HexToHour(RTCC.getMin(RTCC_RTCC));
  te.Hour   = HexToHour(RTCC.getHour(RTCC_RTCC));  // Summer time?
  te.Day    = HexToHour(RTCC.getDate(RTCC_RTCC));
  te.Month  = HexToHour(RTCC.getMonth(RTCC_RTCC));
  te.Year   = HexToHour(RTCC.getYear()) + 30;  // Years since 1970

  // Convert to unixtime
  unixTime = makeTime(te);

  // Set MCU clock to RTCC time, now() returns correct time
  setTime(unixTime);
}
