#pragma once
#include "../setup/modules.h"

bool autonomyActive = false;
bool autonomyStart  = false;
bool autonomyStop   = false;

void DataLogActivate() {
  autonomyStart  = true;
  autonomyActive = true;
}
void DataLogDeactivate() {
  autonomyStop = true;
}

void DataLogStart() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Data Log"));
  DEBUG_PRINTLINE();
  ModuleEnable(MODULE_PWR_CANISTER);
  ModuleEnable(MODULE_COMM_CANISTER);
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Awaiting Handshake... "));
}

void DataLogStop() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Stopping Data Log"));
  DEBUG_PRINTLINE();
  ModuleDisable(MODULE_PWR_CANISTER);
  ModuleDisable(MODULE_COMM_CANISTER);
  autonomyActive = false;
}

void LoggingProcess() {
  if (!autonomyActive) {
    return;
  }
  if (autonomyStart) {
    autonomyStart = false;
    DataLogStart();
  }

  recvWithStartEndMarkersCanister();

  if (autonomyStop) {
    autonomyStop = false;
    DataLogStop();
  }
}