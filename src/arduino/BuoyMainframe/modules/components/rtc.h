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

RTCCI2C RTCC;

bool InitializeRTC() {
  RTCC.begin();

  // enable back up battery
  RTCC.enableVbat();

  // Update Arduino Clock to match RTCC
  UpdateUnixTime();

  // Set Alarm
  InitializeAlarm();

  // Update Arduino Clock to match RTCC
  UpdateUnixTime();

  return RTCC.getYear() != 0;
}

void TerminateRTC() {
  RTCC.stopClock();
}

bool RTCStatus() {
  return GetStatus(MODULE_CLOCK);
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
  // UpdateUnixTime(src);

  // DEBUG_PRINTln(unixTime);

  // print all parameter of the src
  if (src == RTCC_RTCC) {
    DEBUG_PRINT2(RTCC.getMonth(src), HEX);
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT2(RTCC.getDate(src), HEX);
    // year is only available for the RTCC
    if (src == RTCC_RTCC) {
      DEBUG_PRINT(F("/"));
      DEBUG_PRINT2(RTCC.getYear(), HEX);
    }
    DEBUG_PRINT(F(" - "));
  }
  DEBUG_PRINT2(RTCC.getHour(src), HEX);
  DEBUG_PRINT(F(":"));
  DEBUG_PRINT2(RTCC.getMin(src), HEX);

  // second is not supported by the power fail registers
  if (src != RTCC_PWRD && src != RTCC_PWRU) {
    DEBUG_PRINT(F(":"));
    DEBUG_PRINT2(RTCC.getSec(src), HEX);
  }

  DEBUG_PRINTLN();
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
