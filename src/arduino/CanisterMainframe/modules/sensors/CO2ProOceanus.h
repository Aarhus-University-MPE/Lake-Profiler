/*
    CO2 sensor (Pro Oceanus Mini CO2), handles reading sensor output.

    https://pro-oceanus.com/products/mini-series/mini-co2

    Data Format (csv):
      W M,2015,12,02,11,38,14,1676,2139,500.00,503.28,20.697,1007.02,18.40,11.8,4095,2439,1895

    Description:
      Start,Y,M,D,H,M,S,Ref [A/D], Current [A/D], Raw CO2 [ppm], Corrected CO2 [ppmv],
      Sensor Temp [C], Pressure [mBar], IR cell temp [C], Supply Voltage [V], Logger temp [A/D],
      Analog in 1 [A/D], Analog in 2 [A/D], Digital in 1 [0-1], Digital in 2 [0-1]

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#pragma once
#include "../setup/modules.h"

const byte numCharsCO2 = 200;
const byte ppmIndexCO2 = 10;
float co2Concentration = 3.14f;
char dataCO2[numCharsCO2];
char latestDataCO2[numCharsCO2] = "W M,2015,12,02,11,38,14,1676,2139,500.00,503.28,20.697,1007.02,18.40,11.8,4095,2439,1895";
uint8_t packageSizeCO2          = 88;

bool CO2Initialize() {
  COM_CO2.begin(COM_CO2_BAUDRATE);
  return COM_CO2;
}

void CO2Terminate() {
  COM_CO2.end();
}

bool CO2Status() {
  return GetStatus(MODULE_CO2);
}

bool CO2Test() {
  return true;
}

void CO2Read() {
  recvWithStartEndMarkersCO2();
}

// Receive Commands
void recvWithStartEndMarkersCO2() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = 'W';
  char endMarker                = '\n';
  char rc;

  while (COM_CO2.available() > 0) {
    rc = COM_CO2.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        dataCO2[ndx] = rc;
        ndx++;
        if (ndx >= numCharsCO2) {
          ndx = numCharsCO2 - 1;
        }
      } else {
        dataCO2[ndx] = '\0';  // terminate the string
        parseDataCO2(ndx);
        recvInProgress = false;
        ndx            = 0;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
      dataCO2[ndx]   = rc;
      ndx++;
      if (ndx >= numCharsCO2) {
        ndx = numCharsCO2 - 1;
      }
    }
  }
}

// Parses data string and extract desired values (CO2 Concentration)
// TODO: Verify ppmindex extraction correct
void parseDataCO2(uint8_t size) {
  if (dataCO2[2] != 'M') {  // "W M"
    return;
  }
  DEBUG_PRINT(F("Received Data Package: "));
  DEBUG_PRINTLN(dataCO2);

  memset(latestDataCO2, 0, numCharsCO2);
  strcpy(latestDataCO2, dataCO2);

  packageSizeCO2 = size;

  // char tempChars[numCharsCO2];
  // char *strtokIndx;
  // strcpy(tempChars, dataCO2);

  // // Scan forward to ppmIndexCO2, values separated by ","
  // strtokIndx = strtok(tempChars, ",");
  // for (size_t i = 0; i < ppmIndexCO2 - 1; i++) {
  //   strtokIndx = strtok(NULL, ",");
  // }

  // // Convert to float (231546 = 231,546 ppm)
  // co2Concentration = atoi(strtokIndx) / 1000.0f;
}

float GetCo2Concentration() {
  return co2Concentration;
}

// Send latest package
bool CO2SendPackage() {
  return CanisterSendPackage(latestDataCO2, packageSizeCO2);
}
