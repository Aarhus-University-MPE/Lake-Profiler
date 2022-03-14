/*
  GeoRover Backup CPU
  Detects and sends heartbeat to mainframe of GeoRover.
  Resets system if no heartbeat detected within limit.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include "Packages.h"


// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();

  // Debug
  DBG_ONLY(DebugCommInitialize());
  DEBUG_PRINT(F("Debug mode. Entered Setup... "));

  // Setup finished
  DEBUG_PRINTLN(F("Setup complete."));
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  HeartBeat();

  DBG_ONLY(recvWithStartEndMarkersDebug());
}
