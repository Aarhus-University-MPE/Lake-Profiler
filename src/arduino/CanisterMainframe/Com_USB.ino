/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numCharsDebug = 32;
char receivedCMDDebug[numCharsDebug];

bool DebugCommInitialize() {
  COM_SERIAL_PC.begin(DEBUG_BAUDRATE);

  return COM_SERIAL_PC;
}

void DebugCommTerminate() {
  COM_SERIAL_PC.end();
}

bool DebugCommStatus() {
  return GetStatus(MODULE_DEBUGCOMM);
}
// Receive Commands
void recvWithStartEndMarkersDebug() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (COM_SERIAL_PC.available() > 0) {
    rc = COM_SERIAL_PC.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMDDebug[ndx] = rc;
        ndx++;
        if (ndx >= numCharsDebug) {
          ndx = numCharsDebug - 1;
        }
      } else {
        receivedCMDDebug[ndx] = '\0';  // terminate the string
        recvInProgress        = false;
        ndx                   = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// Parse read Command
void parseCommandDebug() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMDDebug);
  DEBUG_PRINTLN(F("\""));
  switch (receivedCMDDebug[0]) {
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}
