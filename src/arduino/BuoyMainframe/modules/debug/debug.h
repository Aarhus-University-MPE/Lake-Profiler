/*
  Debug functions

  Custom Debug functions to execute by sending one of two commands via Serial
    <ME15> or <MD15>
*/
#pragma once
#include "../setup/modules.h"

// Code in this function is executed by sending <ME23>
bool DebugInitialize() {
  // DEBUG_PRINTLINE();
  // DEBUG_PRINTLN(F("Debug Function - Sample Override START"));
  // DEBUG_PRINTLINE();
  // SetClock();
  // DataLogActivate();
  return true;
}

// Code in this function is executed by sending <MD23>
void DebugTerminate() {
  // DEBUG_PRINTLINE();
  // DEBUG_PRINTLN(F("Debug Function - Sample Override HALT"));
  // DEBUG_PRINTLINE();
  // DataLogDeactivate();

  // PowerSet(LOW);
}
