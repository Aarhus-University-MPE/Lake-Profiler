/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "./modules/setup/modules.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();

  // Debug
  DBG_ONLY(SystemEnable(MODULE_COMM_DBG));
  DEBUG_PRINT(F("Debug mode. Entered Setup... "));

  // Setup finished
  DEBUG_PRINTLN(F("Setup complete."));

  SystemEnable(MODULE_BUOY_COMM);
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
    SensorProcess();
  }
}
