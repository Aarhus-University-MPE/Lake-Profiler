/*
  Lake Profiler mainframe
  - Manages primary systems

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021s
*/

#include "./config/_constants.h"
#include "./config/_pinout.h"
#include "./config/_shared.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  InitBlackBox();
  SetStatus(false);

  // Debug
  DBG_ONLY(initializeDebugComm());

  // System initialization
  InitAllPins();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartbeatBlackBox();

  DBG_ONLY(recvWithStartEndMarkers());
}
