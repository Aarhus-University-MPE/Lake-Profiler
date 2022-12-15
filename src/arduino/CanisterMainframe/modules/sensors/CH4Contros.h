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

const byte numCharsCH4         = 200;
const byte ppmEstimateIndexCH4 = 29;
const byte ppmIndexCH4         = 31;
long ch4Concentration          = -3.0f;
long ch4ConcentrationEstimate  = -4.0f;
char dataCH4[numCharsCH4]      = "$CODB4,0,0,D,0,0,2021-03-31,06:18:13,,3,2996,2500000,104594,102668,,,314375,24624172,0,0,1,1334,0,0,0,426987,11338,962,40,7020,341271,342068,337591";

bool CH4Initialize() {
  COM_CH4.begin(COM_CH4_BAUDRATE);

  // delay(50);
  // COM_CH4.print(F("$COCFG,0,0,W,1\r\n"));
  // delay(50);
  // CH4Read();
  // COM_CH4.println(F("1"));
  // delay(50);
  // CH4Read();

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
    // DEBUG_PRINTLN(F("CH4 Data"));
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

// Print CH4 data package
void printDataCH4(uint8_t size) {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("CH4 Data: "));
  for (size_t i = 0; i < size; i++) {
    DEBUG_PRINT(dataCH4[i]);
  }
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("CH4 Concentration: "));
  DEBUG_PRINT(ch4Concentration);
  DEBUG_PRINT(F(", CH4 Concentration Estimate: "));
  DEBUG_PRINTLN(ch4ConcentrationEstimate);
  DEBUG_PRINTLINE();
}

// Parses data string and extract desired values (CH4 Concentration)
void parseDataCH4(uint8_t size) {
  // Verify message contains type being $CODB4
  if (dataCH4[5] != '4') return;

  // Extract concentration values
  ch4Concentration         = ExtractLongFromCharArray(dataCH4, ppmIndexCH4);
  ch4ConcentrationEstimate = ExtractLongFromCharArray(dataCH4, ppmEstimateIndexCH4);

  // Print data package
  DEBUG_PRINTLN(F("CH4 Data"));
  // printDataCH4(size);
}

// Return latest CH4 Concentratione value
long GetCH4Concentration() {
  return ch4Concentration;
}

// Return latest CH4 Concentratione Estimate value
long GetCH4ConcentrationEstimate() {
  return ch4ConcentrationEstimate;
}