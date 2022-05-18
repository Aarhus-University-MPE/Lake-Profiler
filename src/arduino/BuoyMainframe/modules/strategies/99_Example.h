#pragma once

#include "../setup/modules.h"

/*  XX mode

    Description...
*/

// Start sequence of strategy
void StartStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("Strategy (Example): Emergency Starting..."));
  DEBUG_PRINTLINE();

  // Disable all outputs
  ModuleDisable();

  ModuleEnableMode();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Idle): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyExample() {
  //
}

// End sequence of strategy
void FinishStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Ending"));
  DEBUG_PRINTLINE();

  ModuleDisable();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Finished"));
}

// Strategy select button functionality
void SelectFunctionExample() {
  // if (!InputButtonDebounce()) {
  //   return;
  // }

  // Code
}