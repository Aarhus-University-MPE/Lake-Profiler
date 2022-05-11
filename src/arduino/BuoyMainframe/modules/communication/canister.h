
/*
    Canister communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/
#pragma once

#include "../setup/modules.h"

uint8_t receivedCMDCan[numChars];

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

void CanisterAcknowledge() {
  COM_SERIAL_CANISTER.println("<A>");
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

    union unpack pack;
    pack.i8 = rc;

    if (recvInProgressCan == true) {
      if (pack.c != endMarker) {
        receivedCMDCan[ndxCan] = rc;
        ndxCan++;
        if (ndxCan >= numChars) {
          ndxCan = numChars - 1;
        }
      } else {
        receivedCMDCan[ndxCan] = '\0';  // terminate the string
        parseCommandCan(ndxCan);
        recvInProgressCan = false;
        ndxCan            = 0;
      }
    }

    else if (pack.c == startMarker) {
      recvInProgressCan = true;
    }
  }
}

// Parse read Command
void parseCommandCan(uint8_t size) {
  CanisterAcknowledge();

  switch (receivedCMDCan[0]) {
    case 'H':
      DEBUG_PRINTLN(F("Handshake Received"));
      // Send startcommand
      CanisterLogStart();
      break;
    case 0x41:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Received ACKNOWLEDGE"));
      break;
    case 'P':
      parsePackage();
      break;
    default:
      break;
  }
}

void parsePackage() {
  union unpack pack;
  if (receivedCMDCan[2] == 0xA) {
    // new package incomming
    DEBUG_PRINTLINE();
    DEBUG_PRINT(F("New Package Received:"));
    for (uint8_t i = 0; i < receivedCMDCan[1]; i++) {
      DEBUG_PRINT2(receivedCMDCan[i + 2], HEX);
      DEBUG_PRINT(F(" "));
    }
    DEBUG_PRINTLN();
  } else {
    DEBUG_PRINT(F("Message ("));
    DEBUG_PRINT(receivedCMDCan[1]);
    pack.i8 = receivedCMDCan[2];
    DEBUG_PRINT(F(" "));
    DEBUG_PRINT(pack.c);
    DEBUG_PRINTLN(F(")"));
  }

  // for (int i = 0; i < size; i++) {
  //   union unpack pack;

  //   DEBUG_PRINT2(receivedCMDCan[i], HEX);
  //   DEBUG_PRINT(F(" "));
  // }
  // DEBUG_PRINTLN();
}
