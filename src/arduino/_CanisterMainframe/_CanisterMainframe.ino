/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "_constants.h"
#include "_pinout.h"
#include "_shared.h"


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
