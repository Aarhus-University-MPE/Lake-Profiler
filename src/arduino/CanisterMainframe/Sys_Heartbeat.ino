// /*
//   GeoRover Heartbeat functions.
//   Send and receive heartbeat to Buoy.

//   Mads Rosenhøj Jepepsen
//   Aarhus University
//   2021
// */

// unsigned long lastMillisHeartbeatOut, lastMillisHeartbeatIn, lastMillisResetBuoy, lastMillisHeartbeatInt;
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
//   digitalWrite(PO_BUOY_HRTBEAT, false);
//   delay(20);
//   digitalWrite(PO_BUOY_HRTBEAT, true);
// }

// // Checks if time since last heartbeat received > maximum treshold
// // Will attempt to reset Buoy with a frequency of  BUOY_RST_FRQ
// void HeartBeatTimeout() {
//   if (millis() - lastMillisHeartbeatIn > HRTBEAT_TRESHOLD) {
//     // Reset Buoy
//     if (GetStatus(MODULE_BUOY_HRTBEAT)) {
//       lastMillisResetBuoy = millis();
//       SetStatus(MODULE_BUOY_HRTBEAT, false);
//       DEBUG_PRINTLN(F("Error: Buoy offline, attempting to Reset."));
//       ResetBuoy();
//     } else {
//       if (millis() - lastMillisResetBuoy > BACKUP_RST_DT) {
//         lastMillisResetBuoy = millis();
//         DEBUG_PRINTLN(F("Error: Buoy offline, attempting to Reset."));
//         ResetBuoy();
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
//   if (!GetStatus(MODULE_BUOY_HRTBEAT)) {
//     SetStatus(MODULE_BUOY_HRTBEAT, true);
//     DEBUG_PRINTLN(F("Buoy online"));
//   }
// }

// bool HeartBeatStatus() {
//   HeartBeatTimeout();

//   return GetStatus(MODULE_BUOY_HRTBEAT);
// }

// // Sets heartbeat flag
// void HeartBeatInInterrupt() {
//   receivedHeartBeat = true;
// }

// // Attempt to reset BUOY CPU
// void ResetBuoy() {
//   digitalWrite(PO_BUOY_RST, false);
//   delay(20);
//   digitalWrite(PO_BUOY_RST, true);
// }
