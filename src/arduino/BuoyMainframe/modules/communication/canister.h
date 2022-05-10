#pragma once

#include "../setup/modules.h"

/*
    Canister communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/
char receivedCMDCan[numChars];

union unpack {
  uint8_t i8;
  int i;
  char c;
  float f;
  unsigned int ui;
  byte b[4];
};

bool InitializeCanister() {
  COM_SERIAL_CANISTER.begin(CANISTER_BAUDRATE);

  return COM_SERIAL_CANISTER;
}

void TerminateCanister() {
  COM_SERIAL_CANISTER.end();
}

bool CanisterCommStatus() {
  return GetStatus(MODULE_COMM_CANISTER);
}

// Take data log sample
void CanisterLogStart() {
  DEBUG_PRINTLN(F("Sending Start Signal"));
  COM_SERIAL_CANISTER.println("<L>");
}

// Reads data log
void CanisterLogRead() {
  COM_SERIAL_CANISTER.read();
}

// Receive Commands
void recvWithStartEndMarkersCanister() {
  static boolean recvInProgressCan = false;
  static byte ndxCan               = 0;
  char startMarker                 = '<';
  char endMarker                   = '>';
  char rc;

  while (COM_SERIAL_CANISTER.available() > 0) {
    rc = COM_SERIAL_CANISTER.read();
    // DEBUG_PRINT2(rc, HEX);
    // DEBUG_PRINT(F("\t"));
    union unpack pack;

    pack.i8 = rc;

    if (recvInProgressCan == true) {
      if (pack.c != endMarker) {
        receivedCMDCan[ndxCan] = pack.c;
        ndxCan++;
        if (ndxCan >= numChars) {
          ndxCan = numChars - 1;
        }
      } else {
        receivedCMDCan[ndxCan] = '\0';  // terminate the string
        recvInProgressCan      = false;
        ndxCan                 = 0;
        parseCommandCan();
      }
    }

    else if (pack.c == startMarker) {
      recvInProgressCan = true;
    }
  }
}

// Parse read Command
void parseCommandCan() {
  DEBUG_PRINT(F("Received data (Canister): \""));
  for (int i = 0; i < numChars; i++) {
    union unpack pack;

    pack.c = receivedCMDCan[i];
    DEBUG_PRINT2(pack.i8, HEX);
    DEBUG_PRINT(F("\t"));
  }
  DEBUG_PRINTLN(F("\""));

  switch (receivedCMDCan[0]) {
    case 'H':
      DEBUG_PRINT(F("Handshake Received"));
      // Send startcommand
      CanisterLogStart();
      break;

    default:
      break;
  }

  // Log Data
  // receivedCMDCan
}
