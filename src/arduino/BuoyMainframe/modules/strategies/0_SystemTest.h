#pragma once

#include "../setup/modules.h"

/*  Strategy - System Test mode

    Runs system test...
*/
bool systemTestActive;
// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Starting"));

  ModuleDisable();
  systemTestActive = false;

  DEBUG_PRINTLN(F("Strategy (System Test): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  //
  if (!systemTestActive) {
    return;
  }

  recvWithStartEndMarkersCanister();
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Ending"));

  ModuleDisable();
  ModuleEnableMode();

  DEBUG_PRINTLN(F("Strategy (System Test): Finished"));
}

// Strategy function
void SelectFunctionSystemTest() {
  // if (!InputButtonDebounce()) {
  //   return;
  // }
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Activating System Test"));
  DEBUG_PRINTLINE();

  systemTestActive = true;
  // ModuleEnableMode(MODE_AUTONOMOUS);
}
