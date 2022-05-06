/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "_constants.h"
#include "_pinout.h"
#include "_shared.h"

bool systemActive = false;

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
  DBG_ONLY(recvWithStartEndMarkersDebug());

  recvWithStartEndMarkers();  // Scan commands from Buoy

  if (!systemActive) {
    BuoyCommHandshake();
  }

  if (systemActive) {
    //
    SensorProcess();
  }
}
