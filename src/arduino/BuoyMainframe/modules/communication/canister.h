
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
bool dataReceived        = false;
bool acknowledgeReceived = false;

bool InitializeCanister() {
  COM_SERIAL_CANISTER.begin(CANISTER_BAUDRATE);
  dataReceived        = false;
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
  if (!GetStatus(MODULE_PWR_CANISTER) || !GetStatus(MODULE_COMM_CANISTER)) return;

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
  // DEBUG_PRINT(F("Canister Message: "));
  // Serial.write(receivedCMDCan, size, HEX);
  // DEBUG_PRINTLN();
  switch (receivedCMDCan[0]) {
    case 'H':
      DEBUG_PRINTLN(F("Handshake Received"));
      AppendToLog(F("Handshake Received"), true);
      dataReceived = true;
      break;
    case 0x41:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Acknowledge Received"));
      AppendToLog(F("Acknowledge Received"), true);
      acknowledgeReceived = true;
      break;
    case 'P':
      if (receivedCMDCan[1] != size) return;
      parsePackage(size);
      dataReceived = true;
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
      DEBUG_PRINT(CanisterModuleToString(receivedCMDCan[1]));
      DEBUG_PRINTLN();
      AppendToLog(F("Sensor Initialization Error: "));
      AppendToLog(CanisterModuleToString(receivedCMDCan[1]), true);
      // AutonomyStopLog();
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

// returns true if data has been received since last init
bool DataReceived() {
  return dataReceived;
}

bool AcknowledgeReceived() {
  return acknowledgeReceived;
}

// Process data package
void parsePackage(uint8_t size) {
  if (!DataLogActive()) return;

  PrintPackageInfo(size);

  AppendData(size);

  IdStampData();

  TimeStampData();
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
  // Array pointer (include/skip indicator)
  uint8_t *dataPtr = receivedCMDCan + 2;
  AppendPackageIdentifierToData();
  AppendIndexToData();
  AppendToData(dataPtr, size - 2);
}

// Append data to data file
void AppendData() {
  // Array pointer (include/skip indicator)
  char *dataPtr = receivedCMDCanChar + 1;

  AppendToData(dataPtr);

  // Print data to console
  // DEBUG_PRINTLN(dataPtr);
}

// Print latest CH4 Package values
void PrintCH4() {
  union unpack pack;

  // CH4 concentration Estimate
  pack.b[0] = receivedCMDCan[3];
  pack.b[1] = receivedCMDCan[4];
  pack.b[2] = receivedCMDCan[5];
  pack.b[3] = receivedCMDCan[6];

  DEBUG_PRINT(F("CH4 estimate: "));
  DEBUG_PRINTLN(pack.l);

  // CH4 concentration
  pack.b[0] = receivedCMDCan[7];
  pack.b[1] = receivedCMDCan[8];
  pack.b[2] = receivedCMDCan[9];
  pack.b[3] = receivedCMDCan[10];

  DEBUG_PRINT(F("CH4: "));
  DEBUG_PRINTLN(pack.l);
}

// Print lates CO2 Package values
void PrintCO2() {
  union unpack pack;

  // CO2 Raw values
  pack.b[0] = receivedCMDCan[11];
  pack.b[1] = receivedCMDCan[12];
  pack.b[2] = receivedCMDCan[13];
  pack.b[3] = receivedCMDCan[14];

  DEBUG_PRINT(F("CO2 raw: "));
  DEBUG_PRINTLN(pack.l);

  // CO2 concentration
  pack.b[0] = receivedCMDCan[15];
  pack.b[1] = receivedCMDCan[16];
  pack.b[2] = receivedCMDCan[17];
  pack.b[3] = receivedCMDCan[18];

  DEBUG_PRINT(F("CO2: "));
  DEBUG_PRINTLN(pack.l);
}

// Print lates Depth Package values m * 1000
void PrintDepth() {
  union unpack pack;

  pack.b[0] = receivedCMDCan[19];
  pack.b[1] = receivedCMDCan[20];
  pack.b[2] = receivedCMDCan[21];
  pack.b[3] = receivedCMDCan[22];

  DEBUG_PRINT(F("Depth: "));
  DEBUG_PRINTLN(pack.l);
}

// Print lates Temperature Package values
void PrintTemp() {
  union unpack pack;

  pack.b[0] = receivedCMDCan[23];
  pack.b[1] = receivedCMDCan[24];
  pack.b[2] = receivedCMDCan[25];
  pack.b[3] = receivedCMDCan[26];

  DEBUG_PRINT(F("Temp: "));
  DEBUG_PRINTLN(pack.l);
}

// Print lates Luminesence Package values
void PrintLum() {
  union unpack pack;

  pack.b[0] = receivedCMDCan[27];
  pack.b[1] = receivedCMDCan[28];

  DEBUG_PRINT(F("Lum: "));
  DEBUG_PRINTLN(pack.i);
}

// Print lates Package size info
void PrintPackageSize(uint8_t size) {
  DEBUG_PRINT(F("Expected Size: "));
  DEBUG_PRINT(receivedCMDCan[1]);
  DEBUG_PRINT(F(" - Actual Size: "));
  DEBUG_PRINTLN(size);
}

// Print latest package in HEX
void PrintFullPackage(uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    DEBUG_PRINT2(receivedCMDCan[i], HEX);
    DEBUG_PRINT(F(" "));
  }
  DEBUG_PRINTLN();
}

// Print package information
void PrintPackageInfo(uint8_t size) {
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("Sensor Package: "));
  DEBUG_PRINTLN(GetDataLines() + 1);

  // PrintPackageSize(size);
  // PrintFullPackage(size);
  PrintCH4();
  // PrintCO2();
  PrintDepth();
  // PrintTemp();
  // PrintLum();
  // DEBUG_PRINTLINE();
}

long GetLatestDepth() {
  union unpack pack;

  pack.b[0] = receivedCMDCan[19];
  pack.b[1] = receivedCMDCan[20];
  pack.b[2] = receivedCMDCan[21];
  pack.b[3] = receivedCMDCan[22];

  return pack.l;
}
