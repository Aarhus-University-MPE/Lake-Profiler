/*
    CH4 sensor (Contros HydroC CH4), handles reading sensor output.

    https://www.4h-jena.de/en/maritime-technologies/sensors/hydrocrch4/

    RS232 communication converted to UART with MAX232 circuit

    Data Format: (ASCII NMEA-0183)
      DS4 (Data)
      $ CODS4,Dest,Source,D,Format,LDN,... (*Checksum) CR LF

      #29 Conc_estimate - Fraction of CH4 (estimate) in headspace (ppm x 1000)
      #30 pCH4 - Partial pressure of CH4 (µatm x 1000)
      #31 xCH4 - Fraction of CH4 in headspace (ppm x 1000)

      TS4 (Temperature controller data)
      $ COTS1

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#pragma once
#include "../setup/modules.h"

const byte numCharsCH4    = 200;
const byte ppmIndexCH4    = 30;
float ch4Concentration    = -1.0f;
char dataCH4[numCharsCH4] = "$CODB4,0,0,D,0,0,2021-03-31,06:18:13,,3,2996,2500000,104594,102668,,,314375,24624172,0,0,1,1334,0,0,0,426987,11338,962,40,7020,341271,342068,337591";

char latestDataCH4_0[50] = "$CODB4,0,0,D,0,0,2021-03-31,06:18:13,,3,2996,250";
char latestDataCH4_1[50] = "0000,104594,102668,,,314375,24624172,0,0,1,1334,";
char latestDataCH4_2[50] = "0,0,0,426987,11338,962,40,7020,341271,342068,337";
char latestDataCH4_3[50] = "591";

uint8_t packageSizeCH4_0 = 5;
uint8_t packageSizeCH4_1 = 50;
uint8_t packageSizeCH4_2 = 50;
uint8_t packageSizeCH4_3 = 3;

bool CH4Initialize() {
  COM_CH4.begin(COM_CH4_BAUDRATE);
  return COM_CH4;
}

void CH4Terminate() {
  COM_CH4.end();
}

bool CH4Status() {
  return GetStatus(MODULE_CH4);
}

bool CH4Test() {
  return true;
}

void CH4Read() {
  recvWithStartEndMarkersCH4();
}

// Receive Commands
void recvWithStartEndMarkersCH4() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '$';
  char endMarker                = '\n';
  char rc;

  while (COM_CH4.available() > 0) {
    rc = COM_CH4.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        dataCH4[ndx] = rc;
        ndx++;
        if (ndx >= numCharsCH4) {
          ndx = numCharsCH4 - 1;
        }
      } else {
        dataCH4[ndx] = '\0';  // terminate the string
        parseDataCH4(ndx);
        recvInProgress = false;
        ndx            = 0;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
      dataCH4[ndx]   = rc;
      ndx++;
      if (ndx >= numCharsCH4) {
        ndx = numCharsCH4 - 1;
      }
    }
  }
}

// Parses data string and extract desired values (CH4 Concentration)
// TODO: Verify ppmindex extraction correct
void parseDataCH4(uint8_t size) {
  if (dataCH4[5] != '4') {  // $CODB4 contains ppm values
    return;
  }
  // DEBUG_PRINT(F("Received Data Package: "));
  // DEBUG_PRINTLN(dataCH4);

  memset(latestDataCH4_0, 0, 50);
  memset(latestDataCH4_1, 0, 50);
  memset(latestDataCH4_2, 0, 50);
  memset(latestDataCH4_3, 0, 50);

  // TODO: copy data 50 at a time
  // strcpy(latestDataCH4, dataCH4);

  packageSizeCH4_0 = min(50, size);
  packageSizeCH4_0 = min(50, size - 50);
  packageSizeCH4_0 = min(50, size - 100);
  packageSizeCH4_0 = min(50, size - 150);

  char tempChars[numCharsCH4];
  char *strtokIndx;
  strcpy(tempChars, dataCH4);

  // Scan forward to ppmIndexCH4, values separated by ","
  strtokIndx = strtok(tempChars, ",");
  for (size_t i = 0; i < ppmIndexCH4 - 1; i++) {
    strtokIndx = strtok(NULL, ",");
  }

  ch4Concentration = atof(strtokIndx);
}

float GetCH4Concentration() {
  return ch4Concentration;
}

// Send latest package
bool CH4SendPackage() {
  if (!BuoySendPackage(latestDataCH4_0, packageSizeCH4_0)) return false;
  // if (!BuoySendPackage(latestDataCH4_1, packageSizeCH4_1)) return false;
  // if (!BuoySendPackage(latestDataCH4_2, packageSizeCH4_2)) return false;
  // if (!BuoySendPackage(latestDataCH4_3, packageSizeCH4_3)) return false;
  return true;
}
