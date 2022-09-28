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

  // Boot-up system check
  SystemCheck();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  // System strategy Updater
  ModeUpdater();

  // Reattempt LoRa connection
  ModuleEnable(MODULE_COMM_LORA);

  // LoRa Power heartbeat
  LoRaHeartbeat();

  // Primary Strategy
  strategyMethods[1][mode]();

  // System Heartbeat
  HeartbeatBlackBox();

  // USB debug commands
  DBG_ONLY(recvWithStartEndMarkers());
}
