/*
    Canister communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/
char receivedCMDCan[numChars];

bool InitializeCanister() {
  COM_SERIAL_CANISTER.begin(115200);

  return COM_SERIAL_CANISTER;
}

void TerminateCanister() {
  COM_SERIAL_CANISTER.end();
}

bool CanisterCommStatus() {
  return COM_SERIAL_CANISTER;
}

// Take data log sample
void CanisterLogStart() {
  COM_SERIAL_CANISTER.println("<L>");
}

// Reads data log
void CanisterLogRead() {
  COM_SERIAL_CANISTER.read();
}

// Receive Commands
void recvWithStartEndMarkersCanister() {
  static boolean recvInProgressCan = false;
  static byte ndxCan               = 0;
  char startMarker                 = '<';
  char endMarker                   = '>';
  char rc;

  while (COM_SERIAL_CANISTER.available() > 0) {
    rc = COM_SERIAL_CANISTER.read();

    if (recvInProgressCan == true) {
      if (rc != endMarker) {
        receivedCMDCan[ndxCan] = rc;
        ndxCan++;
        if (ndxCan >= numChars) {
          ndxCan = numChars - 1;
        }
      } else {
        receivedCMDCan[ndxCan] = '\0';  // terminate the string
        recvInProgressCan      = false;
        ndxCan                 = 0;
        parseCommandCan();
      }
    }

    else if (rc == startMarker) {
      recvInProgressCan = true;
    }
  }
}

// Parse read Command
void parseCommandCan() {
  DEBUG_PRINT(F("Received data (Canister): \""));
  DEBUG_PRINT(receivedCMDCan);
  DEBUG_PRINTLN(F("\""));

  // Log Data
  // receivedCMDCan
}
