/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];

bool DebugCommInitialize() {
  Serial.begin(DEBUG_BAUDRATE);

  return Serial;
}

void DebugCommTerminate() {
  Serial.end();
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

  while (Serial.available() > 0) {
    rc = Serial.read();

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
void parseCommandDebug() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMD);
  DEBUG_PRINTLN(F("\""));
  switch (receivedCMD[0]) {
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandBackupDebug() {
  switch (receivedCMD[1]) {
    case CMD_BACKUP_RST:
      DEBUG_PRINTLN(F("Manual Reset of Primary System."));
      ResetBuoy();
      break;
    case CMD_BACKUP_BCKUPSTATUS:
      DEBUG_PRINT(F("Primary System Status: "));
      DEBUG_PRINTLN(GetStatus(MODULE_BUOY_HRTBEAT));
      break;
    case CMD_BACKUP_HB:
      DEBUG_PRINTLN(F("Virtual Heartbeat"));
      HeartBeatInInterrupt();
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}
