#pragma once

#include "../setup/modules.h"

/*  Autonomous mode

    Strategy for Autonomous mode
    - Starts logging procedure every specified interval
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting"));
  DEBUG_PRINTLINE();

  // Enable system modules
  ModuleEnableMode();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  LoggingProcess();
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending"));
  DEBUG_PRINTLINE();

  // Disable secondary modules
  ModuleDisable();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished"));
}

// Strategy function
void SelectFunctionAutonomous() {
  if (!InputButtonDebounce()) {
    return;
  }
  //
}