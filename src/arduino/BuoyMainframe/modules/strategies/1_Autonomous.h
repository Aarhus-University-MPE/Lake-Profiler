/*  Autonomous mode

    Strategy for Autonomous mode
    - Starts logging procedure every specified interval
*/

#pragma once
#include "../setup/modules.h"

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting"));
  DEBUG_PRINTLINE();

  // Enable system modules
  ModuleEnableMode();

  // Print current time
  RTCPrint();

  // Set Alarm
  InitializeAlarm();

  AutonomyReset();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  AutonomyProcess();
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending"));
  DEBUG_PRINTLINE();

  // Save current position to EEPROM
  EEPROMSetMotorPos();

  // Disable secondary modules
  ModuleDisable();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished"));
}

// Strategy function
void SelectFunctionAutonomous() {
  // if (!InputButtonDebounce()) return;
}