#pragma once

#include "../setup/modules.h"

/*  Service mode

    Strategy for Service mode.
    - Disables logging
    - Activates motor for surfacing Cansiter
*/

// Start sequence of strategy
void StartStrategyService() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Service): Starting"));
  DEBUG_PRINTLINE();

  ModuleEnableMode();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Service): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyService() {
  // Lift motor to surface
  MotorProcess();
}

// End sequence of strategy
void FinishStrategyService() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Service): Ending"));
  DEBUG_PRINTLINE();

  // Disable secondary modules
  ModuleDisable();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Service): Finished"));
}

// Strategy function
void SelectFunctionService() {
  // if (!InputButtonDebounce()) {
  //   return;
  // }
  //
}