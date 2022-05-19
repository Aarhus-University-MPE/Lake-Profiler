
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
char receivedCMDCanChar[numChars];
bool handshakeReceived   = false;
bool acknowledgeReceived = false;

bool InitializeCanister() {
  COM_SERIAL_CANISTER.begin(CANISTER_BAUDRATE);
  handshakeReceived   = false;
  acknowledgeReceived = false;

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
  AppendToLog(F("Sending Start Signal - \"<L>\""), true);
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Sending Start Signal"));
  DEBUG_PRINTLINE();
  COM_SERIAL_CANISTER.println("<L>");
  acknowledgeReceived = false;
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
        receivedCMDCan[ndxCan]     = rc;
        receivedCMDCanChar[ndxCan] = pack.c;
        ndxCan++;
        if (ndxCan >= numChars) {
          ndxCan = numChars - 1;
        }
      } else {
        receivedCMDCan[ndxCan]     = '\0';  // terminate the string
        receivedCMDCanChar[ndxCan] = '\0';
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
      AppendToLog(F("Handshake Received"), true);
      handshakeReceived = true;
      break;
    case 0x41:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Received ACKNOWLEDGE"));
      AppendToLog(F("Received ACKNOWLEDGE"), true);
      acknowledgeReceived = true;
      break;
    case 'P':
      parsePackage(size);
      break;
    case 'N':
      parseLogStart();
      break;
    // General Error
    case 0x0:
      DEBUG_PRINT(F("General Error: "));
      DEBUG_PRINT2(receivedCMDCan[1], HEX);
      DEBUG_PRINTLN();
      break;
    // Sensor Error
    case 0x1:
      DEBUG_PRINT(F("Sensor Initialization Error: "));
      DEBUG_PRINT2(receivedCMDCan[1], HEX);
      DEBUG_PRINTLN();
      AppendToLog(F("Sensor Initialization Error: "));
      AppendToLog((String)receivedCMDCan[1], true);
      AutonomyStopLog();
      break;
    // Acknowledge Error
    case 0x2:
      DEBUG_PRINTLN(F("Package Acknowledge Error"));
      AppendToLog(F("Package Acknowledge Error"), true);
      AutonomyStopLog();
      break;
    default:
      break;
  }
}

bool HandshakeReceived() {
  return handshakeReceived;
}

bool AcknowledgeReceived() {
  return acknowledgeReceived;
}

void parsePackage(uint8_t size) {
  // PrintPackageInfo(size);

  switch (receivedCMDCan[2]) {
    // New Package
    case 0xA:
      AppendData(size);
      break;
    // End of package
    case 0x13:
      TimeStampData();
      break;
    // CH4 sensor Write as Char Array
    case '$':
      AppendData();
      break;
    // CO2 sensor Write as Char Array
    case 'W':
      AppendData();
      break;
    default:
      // Sensor Package
      AppendData(size);
      break;
  }

  char separator[2] = ";";
  AppendToData(separator);
}

// Create file with current timestamp in name
void parseLogStart() {
  char fileName[9] = "YYMMDDHH";

  GetTimeStamp(fileName);

  // Create data file (YYMMDDHH.csv)
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("New Logging started: "));
  DEBUG_PRINTLN(fileName);
  DEBUG_PRINTLINE();
}

// Append data to data file
void AppendData(uint8_t size) {
  uint8_t *dataPtr = receivedCMDCan + 1;

  AppendToData(dataPtr, size - 1);

  // Print data to console
  // for (int i = 0; i < size - 1; i++) {
  //   DEBUG_PRINT2(dataPtr[i], HEX);
  //   DEBUG_PRINT(F(" "));
  // }
  // DEBUG_PRINTLN();
}

// Append data to data file
void AppendData() {
  char *dataPtr = receivedCMDCanChar + 1;

  AppendToData(dataPtr);

  // Print data to console
  // DEBUG_PRINTLN(dataPtr);
}

// Print package information
void PrintPackageInfo(uint8_t size) {
  union unpack pack;

  DEBUG_PRINTLINE();
  switch (receivedCMDCan[2]) {
    case 0xA:
      DEBUG_PRINT(F("New Package Received:"));
      for (uint8_t i = 0; i < receivedCMDCan[1]; i++) {
        DEBUG_PRINT2(receivedCMDCan[i + 2], HEX);
        DEBUG_PRINT(F(" "));
      }
      DEBUG_PRINT(F("- Package Size: "));
      DEBUG_PRINT(size);
      DEBUG_PRINTLN();
      break;
    case 0x13:
      DEBUG_PRINTLN(F("End of Package"));
      DEBUG_PRINTLINE();
      break;

    default:
      DEBUG_PRINT(F("Sensor Package "));
      DEBUG_PRINT(receivedCMDCan[2]);
      DEBUG_PRINT(F(" - (Expected Size: "));
      DEBUG_PRINT(receivedCMDCan[1]);
      DEBUG_PRINT(F(" - Actual Size: "));
      DEBUG_PRINT(size);
      pack.i8 = receivedCMDCan[2];
      DEBUG_PRINT(F(" "));
      DEBUG_PRINT(pack.c);
      DEBUG_PRINTLN(F(")"));
      break;
  }
}