/*
    CO2 sensor (Pro Oceanus Mini CO2), handles reading sensor output.

    https://pro-oceanus.com/products/mini-series/mini-co2

    Data Format (csv):
      W M,2015,12,02,11,38,14,1676,2139,500.00,503.28,20.697,1007.02,18.40,11.8,4095,2439,1895

      500.00, 503.28

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

const byte numCharsCO2    = 200;
const byte ppmIndexCO2Raw = 10;
const byte ppmIndexCO2    = 11;
long co2Concentration     = -1;
long co2Raw               = -2;

char dataCO2[numCharsCO2] = "W M,2015,12,02,11,38,14,1676,2139,500.00,503.28,20.697,1007.02,18.40,11.8,4095,2439,1895";

bool CO2Initialize() {
  COM_CO2.begin(COM_CO2_BAUDRATE);

  // delay(50);
  COM_CO2.write(0x1b);  // Initiate Communication
  delay(50);
  COM_CO2.write(0x31);  // Sensor ON

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

// Print CO2 data package
void PrintCO2Payload(uint8_t size) {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("CO2 Data: "));
  for (size_t i = 0; i < size; i++) {
    DEBUG_PRINT(dataCO2[i]);
  }
  DEBUG_PRINTLN();
}

void PrintDataCO2() {
  DEBUG_PRINT(F("CO2 Concentration: "));
  DEBUG_PRINT(co2Concentration);
  DEBUG_PRINT(F(", CO2 Raw: "));
  DEBUG_PRINTLN(co2Raw);
}

// Parses data string and extract desired values (CO2 Concentration)
void parseDataCO2(uint8_t size) {
  // Verify message contains type ("W M")
  if (dataCO2[2] != 'M') return;

  // Extract concentration values
  co2Concentration = ExtractLongFromCharArray(dataCO2, ppmIndexCO2, 1000.0f);
  co2Raw           = ExtractLongFromCharArray(dataCO2, ppmIndexCO2Raw, 1000.0f);

  // Print data package
  // DEBUG_PRINTLN(F("CO2 Data"));
  // PrintCO2Payload(size);
  // PrintDataCO2();
}

long GetCo2Concentration() {
  return co2Concentration;
}

long GetCo2Raw() {
  return co2Raw;
}