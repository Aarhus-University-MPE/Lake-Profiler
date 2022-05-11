/*
    RS232 communication, handles reading serial and parsing commands.
    Transmits data to and from Buoy.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#pragma once
#include "../setup/modules.h"

const byte numChars = 32;
char receivedCMD[numChars];
bool acknowledgeReceived = false;

bool BuoyCommInitialize() {
  COM_BUOY.begin(BUOY_BAUDRATE);

  return COM_BUOY;
}

void BuoyCommTerminate() {
  COM_BUOY.end();
}

unsigned long lastMillisHandshake;
void BuoyCommHandshake() {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return;
  }
  if (millis() - lastMillisHandshake < HANDSHAKE_PERIOD) {
    return;
  }
  lastMillisHandshake = millis();
  DEBUG_PRINTLN(F("Sending Handshake"));
  digitalWrite(LED_BUILTIN, true);
  COM_BUOY.println("<H>");
  delay(50);
  digitalWrite(LED_BUILTIN, false);
}

void BuoyCommAck() {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return;
  }
  DEBUG_PRINTLN(F("Sending ACK"));
  COM_BUOY.println("<A>");
}
void BuoyCommNack() {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return;
  }
  DEBUG_PRINTLN(F("Sending Nack"));
  COM_BUOY.println("<N>");
}
// Receive Commands
void recvWithStartEndMarkers() {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return;
  }

  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (COM_BUOY.available() > 0) {
    rc = COM_BUOY.read();

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

// Parse read Command
void parseCommand() {
  char *sampleRatePtr           = receivedCMD + 1;
  char sampleRateChar[numChars] = {0};
  strcpy(sampleRateChar, sampleRatePtr);

  int sampleRate = atoi(sampleRateChar);

  switch (receivedCMD[0]) {
    case CMD_LOGGING:
      parseCommandLog();
      break;
    case CMD_SAMPLERATE:
      SetSampleInterval(sampleRate);
      break;
    case 'A':
      acknowledgeReceived = true;
    case '\0':
      break;
    default:
      break;
  }
}

void parseCommandLog() {
  BuoyCommAck();
  LoggingStart();
}

void CanisterSendStart(uint8_t size) {
  COM_BUOY.print("<P");
  uint8_t sizePackage[1];
  sizePackage[0] = size;
  COM_BUOY.write(sizePackage, 1);
}

void CanisterSendStart() {
  COM_BUOY.print("<");
}
void CanisterSendEndline() {
  COM_BUOY.print(">\r\n");
}

void ResetAcknowledge() {
  acknowledgeReceived = false;
}

bool CanisterSendPackage(uint8_t package[], uint8_t size) {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return false;
  }

  PrintPackage(package, size);

  CanisterSendStart(size);
  COM_BUOY.write(package, size);
  unsigned long lastMillisSend = millis();
  CanisterSendEndline();
  ResetAcknowledge();

  delay(10);
  recvWithStartEndMarkers();

  int retry = 0;
  while (!acknowledgeReceived && retry < 3) {
    recvWithStartEndMarkers();
    if (millis() - lastMillisSend > 200) {
      retry++;
      DEBUG_PRINTLN(F("Failed to received ACK, resending"));
      CanisterSendStart(size);
      COM_BUOY.write(package, size);
      CanisterSendEndline();
      lastMillisSend = millis();
    }
  }

  if (!acknowledgeReceived) {
    DEBUG_PRINTLN(F("Failed to received ACK"));
    // Stop log report error
    CanisterSendCommunicationError();
    LoggingStop();
    return false;
  }

  return true;
}

bool CanisterSendPackage(char package[], uint8_t size) {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return false;
  }
  // PrintPackage(package, size);

  union unpack pack;
  uint8_t packagei8[size];
  for (size_t i = 0; i < size; i++) {
    pack.c       = package[i];
    packagei8[i] = pack.i8;
  }

  return CanisterSendPackage(packagei8, size);
}

void CanisterSendCommunicationError() {
  CanisterSendStart();
  COM_BUOY.print(PACKAGE_ERROR_COMM_ACK);
  CanisterSendEndline();
}

void CanisterSendSensorError(byte sensorModule) {
  if (!GetStatus(MODULE_BUOY_COMM)) {
    DEBUG_PRINTLN(F("Module Not Active!"));
    return;
  }

  uint8_t size = 2;
  uint8_t errorPackage[size];

  errorPackage[0] = PACKAGE_ERROR_SENSOR;
  errorPackage[1] = sensorModule;

  PrintPackage(errorPackage, size);

  CanisterSendStart();
  COM_BUOY.write(errorPackage, size);
  CanisterSendEndline();
}

void PrintPackage(uint8_t package[], uint8_t size) {
  // DEBUG_PRINTLN(F("Sending Package uint8: "));

  DEBUG_PRINT(F("Message ("));
  DEBUG_PRINT(size);
  DEBUG_PRINT(F("): "));

  // Print package
  // for (uint8_t i = 0; i < size; i++) {
  //   DEBUG_PRINT2(package[i], HEX);
  //   DEBUG_PRINT(F(" "));
  // }

  DEBUG_PRINTLN();
  // DEBUG_PRINTLINE();
}

void PrintPackage(char package[], uint8_t size) {
  // DEBUG_PRINTLN(F("Sending Package char: "));

  // Print package
  for (uint8_t i = 0; i < size; i++) {
    DEBUG_PRINT(package[i]);
  }

  DEBUG_PRINTLN();
  // DEBUG_PRINTLINE();
}

bool BuoyCommStatus() {
  return GetStatus(MODULE_BUOY_COMM);
}

bool BuoyCommTest() {
  return true;
}
