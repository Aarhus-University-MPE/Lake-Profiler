/*
  GeoRover Heartbeat functions.
  Send and receive heartbeat to Buoy.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <Arduino.h>

unsigned long lastMillisHeartbeatOut, lastMillisHeartbeatIn, lastMillisResetBuoy, lastMillisHeartbeatInt;
bool receivedHeartBeat;

// Runs heartbeat in and out
void HeartBeat();

// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut();

// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset Buoy with a frequency of  BUOY_RST_FRQ
void HeartBeatTimeout();

void HeartBeatIn();

bool HeartBeatStatus();

// Sets heartbeat flag
void HeartBeatInInterrupt();

// Attempt to reset BUOY CPU
void ResetBuoy();