/*
    RS232 communication, handles reading serial and parsing commands.
    Transmits data to and from Buoy.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];

bool BuoyCommInitialize() {
  COM_BUOY.begin(BUOY_BAUDRATE);

  return COM_BUOY;
}

void BuoyCommTerminate() {
  COM_BUOY.end();
}

unsigned long lastMillisHandshake;
void BuoyCommHandshake() {
  if (millis() - lastMillisHandshake < HANDSHAKE_PERIOD) {
    return;
  }
  lastMillisHandshake = millis();
  DEBUG_PRINTLN(F("Sending Handshake"));
  COM_BUOY.println("<R>");
}

void BuoyCommAck() {
  DEBUG_PRINTLN(F("Sending ACK"));
  COM_BUOY.println("<A>");
}
void BuoyCommNack() {
  DEBUG_PRINTLN(F("Sending Nack"));
  COM_BUOY.println("<N>");
}
// Receive Commands
void recvWithStartEndMarkers() {
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
  switch (receivedCMD[0]) {
    case CMD_LOGGING:
      parseCommandLog();
      break;
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

void CanisterSendPackage(uint8_t package[], uint8_t size) {
  DEBUG_PRINTLN(F("Sending Package!"));
  COM_BUOY.println(F("<EMPTY_PACKAGE>"));  // TODO: Remove Temporary package placeholder
  COM_BUOY.write(package, size);
}

bool BuoyCommStatus() {
  return GetStatus(MODULE_BUOY_COMM);
}

bool BuoyCommTest() {
  return true;
}
