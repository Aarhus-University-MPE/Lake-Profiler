/*
  Lake Profiler mainframe
  - Manages primary systems

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021s
*/

#include "./modules/setup/modules.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  SetStatus(false);
  DBG_ONLY(ModuleEnable(MODULE_COMM_DBG));

  // System initialization
  InitAllPins();

  SystemEnablePrimary();

  SystemCheck();

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
