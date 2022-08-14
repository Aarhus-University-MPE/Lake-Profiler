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
  DEBUG_PRINTLN(F("Debug mode. Entered Setup... "));

  SystemEnable(MODULE_BUOY_COMM);

  // Enable sensors
  SystemEnablePrimary();

  // Setup sample configuration
  ReadSampleInterval();

  // Setup finished
  DEBUG_PRINTLN(F("Setup complete."));
  DEBUG_PRINTLINE();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  DBG_ONLY(recvWithStartEndMarkersDebug());  // Scan commands from USB (Serial)

  recvWithStartEndMarkers();  // Scan commands from Buoy

  SensorProcess();  // Read/Broadcast sensor data
}
