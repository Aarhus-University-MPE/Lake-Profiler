/*
    RS232 communication, handles reading serial and parsing commands.
    Transmits data to and from Buoy.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numCharsDbg = 32;
char receivedCMDDBG[numCharsDbg];

bool BuoyCommInitialize() {
  Serial.begin(BUOY_BAUDRATE);

  return Serial;
}

void BuoyCommTerminate(){
  Serial.end();
}

// Receive Commands
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (Serial.available() > 0) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMDDBG[ndx] = rc;
        ndx++;
        if (ndx >= numCharsDbg) {
          ndx = numCharsDbg - 1;
        }
      } else {
        receivedCMDDBG[ndx] = '\0';  // terminate the string
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
  switch (receivedCMDDBG[0]) {
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case '\0':
      break;
    default:
      break;
  }
}

void parseCommandBackup() {
  switch (receivedCMDDBG[1]) {
    case CMD_BACKUP_RST:
      ResetBuoy();
      break;
    case CMD_BACKUP_BCKUPSTATUS:
      DEBUG_PRINTLN(GetStatus(MODULE_BUOY_HRTBEAT));
      break;
    case CMD_BACKUP_HB:
      HeartBeatInInterrupt();
      break;
    default:
      break;
  }
}

bool BuoyCommStatus(){
  return GetStatus(MODULE_BUOY_COMM);
}

bool BuoyCommTest(){
  return true;
}
