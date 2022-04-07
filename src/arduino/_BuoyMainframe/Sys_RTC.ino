/*
    Real Time Clock handles timing.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

bool InitializeRTC() {
  return true;
}

bool TerminateRTC() {
  return true;
}

bool RTCStatus(){
  return GetStatus(MODULE_CLOCK);
}