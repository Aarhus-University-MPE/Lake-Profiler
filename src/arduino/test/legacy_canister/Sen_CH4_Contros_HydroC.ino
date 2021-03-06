/*
    CH4 sensor (Contros HydroC CH4), handles reading sensor output.

    https://www.4h-jena.de/en/maritime-technologies/sensors/hydrocrch4/

    RS232 communication converted to UART with MAX232 circuit

    Data Format: (ASCII NMEA-0183)
      DS4 (Data)
      $ CODS4,Dest,Source,D,Format,LDN,... (*Checksum) CR LF

      #31 Conc_estimate - Fraction of CH4 (estimate) in headspace (ppm x 1000)

      TS4 (Temperature controller data)
      $ COTS1

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

const byte numCharsCH4 = 200;
const byte ppmIndexCH4 = 30;
float ch4Concentration = 3.14f;
char dataCH4[numCharsCH4];

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
        dataCH4[ndx]   = '\0';  // terminate the string
        recvInProgress = false;
        ndx            = 0;
        parseDataCH4();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// Parses data string and extract desired values (CH4 Concentration)
// TODO: Verify ppmindex extraction correct
void parseDataCH4() {
  if (dataCH4[4] != '4') {  // $CODB4 contains ppm values
    return;
  }
  DEBUG_PRINT(F("Received Data Package: "));
  DEBUG_PRINTLN(dataCH4);

  char tempChars[numCharsCH4];
  char *strtokIndx;
  strcpy(tempChars, dataCH4);

  // Scan forward to ppmIndexCH4, values separated by ","
  strtokIndx = strtok(tempChars, ",");
  for (size_t i = 0; i < ppmIndexCH4 - 1; i++) {
    strtokIndx = strtok(NULL, ",");
  }

  // Convert to float (231546 = 231,546 ppm)
  ch4Concentration = atoi(strtokIndx) / 1000.0f;
}

float GetCH4Concentration() {
  return ch4Concentration;
}