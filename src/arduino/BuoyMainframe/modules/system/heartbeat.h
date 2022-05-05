#pragma once

#include "../setup/modules.h"

// /*
//   GeoRover Heartbeat functions.
//   Send and receive heartbeat to Canister CPU.

//   Mads Rosenhøj Jepepsen
//   Aarhus University
//   2021
// */

// unsigned long lastMillisHeartbeatOut, lastMillisHeartbeatIn, lastMillisResetCanister, lastMillisHeartbeatInt;
// bool receivedHeartBeat;

// // Runs heartbeat in and out
// void HeartBeat() {
//   HeartBeatOut();
//   HeartBeatIn();
//   HeartBeatTimeout();
// }

// // Sends heartbeat with with frequency of HRTBEAT_FRQ
// void HeartBeatOut() {
//   if (millis() - lastMillisHeartbeatOut < HRTBEAT_DT_OUT) {
//     return;
//   }
//   DEBUG_PRINTLN(F("Sending Heartbeat!"));
//   lastMillisHeartbeatOut = millis();
//   digitalWrite(PO_CANISTER_HRTBEAT, false);
//   delay(20);
//   digitalWrite(PO_CANISTER_HRTBEAT, true);
// }

// // Checks if time since last heartbeat received > maximum treshold
// // Will attempt to reset Canister CPU with a frequency of  CANISTER_RST_FRQ
// void HeartBeatTimeout() {
//   if (millis() - lastMillisHeartbeatIn > HRTBEAT_TRESHOLD) {
//     // Reset Canister CPU
//     if (GetStatus(MODULE_CANISTER_HRTBEAT)) {
//       lastMillisResetCanister = millis();
//       SetStatus(MODULE_CANISTER_HRTBEAT, false);
//       DEBUG_PRINTLN(F("Error: Canister System offline, attempting to Reset."));
//       ResetCanisterCPU();
//     } else {
//       if (millis() - lastMillisResetCanister > BACKUP_RST_DT) {
//         lastMillisResetCanister = millis();
//         DEBUG_PRINTLN(F("Error: Canister offline, attempting to Reset."));
//         ResetCanisterCPU();
//       }
//     }
//   }
// }

// void HeartBeatIn() {
//   if (!receivedHeartBeat) {
//     return;
//   }
//   receivedHeartBeat = false;

//   if (millis() - lastMillisHeartbeatIn < BTN_DEBOUNCE_TIME) {
//     return;
//   }

//   lastMillisHeartbeatIn = millis();
//   DEBUG_PRINTLN(F("Heartbeat Received!"));
//   if (!GetStatus(MODULE_CANISTER_HRTBEAT)) {
//     SetStatus(MODULE_CANISTER_HRTBEAT, true);
//     DEBUG_PRINTLN(F("Canister online"));
//   }
// }

// // Sets heartbeat flag
// void HeartBeatInInterrupt() {
//   receivedHeartBeat = true;
// }

// // Attempt to reset Canister CPU
// void ResetCanisterCPU() {
//   digitalWrite(PO_CANISTER_RST, false);
//   delay(20);
//   digitalWrite(PO_CANISTER_RST, true);
// }
