#pragma once

#include "../setup/modules.h"

byte mode, modeCycle, prevMode;
boolean isModeUpdated = false;

// Set pointers for strategies methods
void InitStrategyMethods() {
  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;
  strategyMethods[3][MODE_SYSTEMTEST] = SelectFunctionSystemTest;

  strategyMethods[0][MODE_AUTONOMOUS] = StartStrategyAutonomous;
  strategyMethods[1][MODE_AUTONOMOUS] = RunStrategyAutonomous;
  strategyMethods[2][MODE_AUTONOMOUS] = FinishStrategyAutonomous;
  strategyMethods[3][MODE_AUTONOMOUS] = SelectFunctionAutonomous;

  strategyMethods[0][MODE_SERVICE] = StartStrategyService;
  strategyMethods[1][MODE_SERVICE] = RunStrategyService;
  strategyMethods[2][MODE_SERVICE] = FinishStrategyService;
  strategyMethods[3][MODE_SERVICE] = SelectFunctionService;
}

// Reads mode selector pins
byte ModeRead() {
  byte readMode;
  if (digitalRead(PI_SWITCH_MODE)) {
    readMode = MODE_SERVICE;
  } else {
    readMode = MODE_AUTONOMOUS;
  }
  return readMode;
}

// Initialize system mode
void InitMode() {
  mode = ModeRead();

  strategyMethods[0][mode]();
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater() {
  if (!isModeUpdated) {
    return;
  }

  if (!SetMode()) {
    return;
  }

  isModeUpdated = false;

  strategyMethods[2][prevMode]();  // finish any operations for prevMode here

  strategyMethods[0][mode]();  // init new strategy according to the new mode value
}

// Tries set the mode and isModeUpdated flag
bool SetMode() {
  prevMode = mode;
  mode     = ModeRead();

  // EEPROM.write(MEMADDR_LASTMODE, mode);
  DEBUG_PRINT(F("Mode is set to: "));
  DEBUG_PRINTLN(ModeToString(mode));
  return true;
}

byte GetMode() {
  return mode;
}

void ModeSwitchInterrupt() {
  if (!ModeButtonDebounce()) {
    return;
  }

  isModeUpdated = true;
}
