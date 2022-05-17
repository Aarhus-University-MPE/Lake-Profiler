#pragma once

#include "../setup/modules.h"

/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

char receivedCMD[numChars];

bool InitializeDebugComm() {
  COM_SERIAL_DBG.begin(DEBUG_BAUDRATE);

  if (COM_SERIAL_DBG) {
    DEBUG_PRINTLN(F("#--------------------------------#"));
    DEBUG_PRINTLN(F("#     Lake-Profiler Firmware     #"));
    DBG_ONLY(DEBUG_PRINTLN(F("#          #DEBUG MODE#          #")));
    DEBUG_PRINT(F("#       System Version: "));
    DEBUG_PRINT(SystemVersion);
    DEBUG_PRINTLN(F("   #"));
    DEBUG_PRINTLN(F("#--------------------------------#"));
    DEBUG_PRINTLN();
  }

  return COM_SERIAL_DBG;
}

// Receive Commands
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (COM_SERIAL_DBG.available() > 0) {
    rc = COM_SERIAL_DBG.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMD[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedCMD[ndx] = '\0';  // terminate the string
        recvInProgress   = false;
        ndx              = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

bool DebugCommStatus() {
  return COM_SERIAL_DBG;
}

// Parse read Command
void parseCommand() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMD);
  DEBUG_PRINTLN(F("\""));

  switch (receivedCMD[0]) {
    case CMD_FILES:
      parseCommandFiles();
      break;
    case CMD_STRATEGY:
      parseCommandStrategy();
      break;
    case CMD_MODULE:
      parseCommandModule();
    case CMD_CLOCK:
      parseCommandClock();
      break;
    case CMD_ALARM:
      parseCommandAlarm();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandFiles() {
  char *fileNamePtr       = receivedCMD + 2;
  char fileName[numChars] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receivedCMD[1]) {
    case CMD_FILES_LIST:
      SDQuery();
      break;
    case CMD_FILES_SIZE:
      SDSize(fileName);
      break;
    case CMD_FILES_DOWNLOAD:
      SDDownload(fileName);
      break;
    case CMD_FILES_CREATE:
      SDCreate(fileName);
      break;
    case CMD_FILES_REMOVE:
      SDDelete(fileName);
      break;
    case CMD_FILES_WRITE:
      SDWriteStream(fileName);
      break;
    case CMD_FILES_WRITENEWLINE:
      SDWriteStreamNewLine();
      break;
    case CMD_FILES_QUIT:
      SDQuit();
      break;
    case CMD_FILES_BLCKBOX:
      BlackBoxPrint();
      break;
    case CMD_FILES_BLCKBOXEMPTY:
      BlackBoxEmpty();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandStrategy() {
  switch (receivedCMD[1]) {
    case CMD_STRATEGY_SET:
      // DEBUG_PRINT(F("Manual Strategy Set to: "));
      // DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
      // if (!SetModeManual((int)(receivedCMD[2] - '0')))
      //   DEBUG_PRINTLN(F("Mode not found!"));
      break;
    case CMD_STRATEGY_FUNCTION:
      strategyMethods[3][GetMode()]();
      break;
    case CMD_STRATEGY_OVERRIDE:
      // Override
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandModule() {
  char *modulePtr           = receivedCMD + 2;
  char moduleChar[numChars] = {0};
  strcpy(moduleChar, modulePtr);

  int moduleSlct = atoi(moduleChar);

  switch (receivedCMD[1]) {
    case CMD_MODULE_ENABLE:
      DEBUG_PRINT(F("Manual Enable of Module: "));
      switch (receivedCMD[2]) {
        default:
          DEBUG_PRINTLN(moduleSlct);
          ModuleEnable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_DISABLE:
      DEBUG_PRINT(F("Manual Disable of Module: "));
      switch (receivedCMD[2]) {
        case '\0':
          DEBUG_PRINTLN(F("ALL SYSTEMS"));
          ModuleDisable();
          break;
        default:
          DEBUG_PRINTLN(moduleSlct);
          ModuleDisable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_OVERRIDE:
      DEBUG_PRINT(F("Manual Override of Module: "));
      switch (receivedCMD[2]) {
        case '\0':
          DEBUG_PRINTLN(F("NACK"));
          // ModuleDisable();
          break;
        default:
          DEBUG_PRINT(moduleSlct);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINTLN(ToBoolString(!GetStatus(moduleSlct)));
          SetStatus(moduleSlct, !GetStatus(moduleSlct));
          break;
      }
      break;
    case CMD_MODULE_STATUS:
      DEBUG_PRINTLN(F("Manual System Status Check"));
      // GetStatus(true);
      DEBUG_PRINT(F("System Status: "));
      DEBUG_PRINTLN(String(ToLong(SystemStatus)));
      break;
    case CMD_MODULE_RESET:
      DEBUG_PRINT(F("Manual System Reset in: "));
      CountDownPrint();
      systemReset();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandClock() {
  char *timePtr           = receivedCMD + 2;
  char timeChar[numChars] = {0};
  strcpy(timeChar, timePtr);

  uint8_t clockValue    = atoi(timeChar);
  uint8_t clockValueHex = TimeToHex(clockValue);
  // uint8_t clockValue = receivedCMD[2] - '0';

  DEBUG_PRINT(F("Setting Clock Value: "));
  switch (receivedCMD[1]) {
    case CMD_CLOCK_SEC:
      DEBUG_PRINT(F("sec "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockSec(clockValueHex);
      break;
    case CMD_CLOCK_MIN:
      DEBUG_PRINT(F("min "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockMin(clockValueHex);
      break;
    case CMD_CLOCK_HOUR:
      DEBUG_PRINT(F("Hour "));
      DEBUG_PRINT2(clockValueHex, HEX);
      DEBUG_PRINT(F(":00"));
      SetClockHour(clockValueHex);
      break;
    case CMD_CLOCK_DAY:
      DEBUG_PRINT(F("Day "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockDay(clockValueHex);
      break;
    case CMD_CLOCK_DATE:
      DEBUG_PRINT(F("Date "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockDate(clockValueHex);
      break;
    case CMD_CLOCK_MONTH:
      DEBUG_PRINT(F("Month "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockMonth(clockValueHex);
      break;
    case CMD_CLOCK_YEAR:
      DEBUG_PRINT(F("Year "));
      DEBUG_PRINT2(clockValueHex, HEX);
      SetClockYear(clockValueHex);
      break;
    case CMD_CLOCK_PRINT:
      DEBUG_PRINTLN(F("Print"));
      RTCPrint();
      break;

    default:
      break;
  }
  DEBUG_PRINTLN();

  // Update Arduino Clock to match RTCC
  UpdateUnixTime();
}

void parseCommandAlarm() {
  char *timePtr           = receivedCMD + 2;
  char timeChar[numChars] = {0};
  strcpy(timeChar, timePtr);

  uint8_t clockValue    = atoi(timeChar);
  uint8_t clockValueHex = HourToHex(clockValue);

  DEBUG_PRINT(F("Setting Alarm: "));
  switch (receivedCMD[1]) {
    case CMD_CLOCK_HOUR:
      DEBUG_PRINT(F("Hour "));
      DEBUG_PRINT2(clockValueHex, HEX);
      DEBUG_PRINT(F(":00"));
      SetAlarmHour(clockValueHex);
      break;
    case CMD_ALARM_FREQUENCY:
      DEBUG_PRINT(F("Frequency: "));
      DEBUG_PRINT(clockValue);
      SetAlarmFrequency(clockValue);
      break;
    case CMD_ALARM_START_HOUR:
      DEBUG_PRINT(F("Start Hour: "));
      DEBUG_PRINT2(clockValueHex, HEX);
      DEBUG_PRINT(F(":00"));
      SetAlarmStartHour(clockValueHex);
      break;
    case CMD_ALARM_BEGIN:
      DEBUG_PRINT(F("ON"));
      DEBUG_PRINTLN();
      SetAlarm();
      break;

    default:
      break;
  }
  DEBUG_PRINTLN();
}

void CountDownPrint() {
  DEBUG_PRINT(F("3"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("2"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("1"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINTLN(F(". "));
  delay(333);
}