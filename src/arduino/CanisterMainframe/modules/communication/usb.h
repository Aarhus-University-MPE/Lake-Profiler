/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#pragma once
#include "../setup/modules.h"

const byte numCharsDebug = 32;
char receivedCMDDebug[numCharsDebug];

bool DebugCommInitialize() {
  COM_SERIAL_PC.begin(DEBUG_BAUDRATE);

  DEBUG_PRINTLN(F("#--------------------------------#"));
  DEBUG_PRINTLN(F("#     Lake-Profiler Firmware     #"));
  DEBUG_PRINTLN(F("#           Canister             #"));
  DBG_ONLY(DEBUG_PRINTLN(F("#          #DEBUG MODE#          #")));
  DEBUG_PRINT(F("#       System Version: "));
  DEBUG_PRINT(SystemVersion);
  DEBUG_PRINTLN(F("   #"));
  DEBUG_PRINTLN(F("#--------------------------------#"));
  DEBUG_PRINTLN();

  return COM_SERIAL_PC;
}

void DebugCommTerminate() {
  COM_SERIAL_PC.end();
}

bool DebugCommStatus() {
  return GetStatus(MODULE_COMM_DBG);
}
// Receive Commands
void recvWithStartEndMarkersDebug() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (COM_SERIAL_PC.available() > 0) {
    rc = COM_SERIAL_PC.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMDDebug[ndx] = rc;
        ndx++;
        if (ndx >= numCharsDebug) {
          ndx = numCharsDebug - 1;
        }
      } else {
        receivedCMDDebug[ndx] = '\0';  // terminate the string
        recvInProgress        = false;
        ndx                   = 0;
        parseCommandDebug();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// Parse read Command
void parseCommandDebug() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMDDebug);
  DEBUG_PRINTLN(F("\""));

  char *sampleRatePtr                = receivedCMDDebug + 1;
  char sampleRateChar[numCharsDebug] = {0};
  strcpy(sampleRateChar, sampleRatePtr);

  int sampleRate = atoi(sampleRateChar);

  switch (receivedCMDDebug[0]) {
    case CMD_SAMPLERATE:
      SetSampleInterval(sampleRate);
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}
