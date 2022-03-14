/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#include "Arduino.h"

const byte numChars = 32;
char receivedCMD[numChars];

bool DebugCommInitialize();

void DebugCommTerminate();

bool DebugCommStatus();

// Receive Commands
void recvWithStartEndMarkersDebug();

// Parse read Command
void parseCommandDebug();

void parseCommandBackupDebug();