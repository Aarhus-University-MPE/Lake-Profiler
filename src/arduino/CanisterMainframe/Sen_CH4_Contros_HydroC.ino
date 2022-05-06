/*
    CH4 sensor (Contros HydroC CH4), handles reading sensor output.

    https://www.4h-jena.de/en/maritime-technologies/sensors/hydrocrch4/

    RS232 communication converted to UART with MAX232 circuit

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

const byte numCharsCH4 = 200;
const byte ppmIndex    = 30;
float ch4Concentration;
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

  while (COM_BUOY.available() > 0) {
    rc = COM_BUOY.read();

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

  // Scan forward to ppmIndex, values separated by ","
  strtokIndx = strtok(tempChars, ",");
  for (size_t i = 0; i < ppmIndex - 1; i++) {
    strtokIndx = strtok(NULL, ",");
  }

  // Convert to float (231546 = 231,546 ppm)
  ch4Concentration = atoi(strtokIndx) / 1000.0f;
}