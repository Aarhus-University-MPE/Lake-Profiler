/*
    RS232 communication, handles reading serial and parsing commands.
    Transmits data to and from Buoy.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#include "Arduino.h"

const byte numCharsDbg = 32;
char receivedCMDDBG[numCharsDbg];

bool BuoyCommInitialize();

void BuoyCommTerminate();

// Receive Commands
void recvWithStartEndMarkers();

// Parse read Command
void parseCommand();

void parseCommandBackup();

bool BuoyCommStatus();

bool BuoyCommTest();
