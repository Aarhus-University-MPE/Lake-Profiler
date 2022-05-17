/*  Autonomous Process

    Primary processes for autonomous modes
*/
#pragma once
#include "../setup/modules.h"

char fileLocationData[22];
char fileLocationLog[22];

// Create logging files (YYMMDDHH.log and YYMMDDHH.csv)
bool InitializeLoggingFiles() {
  char folder[8]   = "./data/";
  char fileName[9] = "YYMMDDHH";
  char fileEnd[5]  = ".log";

  // Get current time for data name
  GetTimeStamp(fileName);

  // Clear file names
  memset(fileLocationData, 0, 22);
  memset(fileLocationLog, 0, 22);

  // Combine folder, filename and format
  strcpy(fileLocationData, folder);
  strcat(fileLocationData, fileName);

  // Create data file
  if (!SDCreate(fileLocationData)) {
    return false;
  }

  // Append .log
  strcat(fileLocationLog, fileLocationData);
  strcat(fileLocationLog, fileEnd);

  // Create log file
  if (!SDCreate(fileLocationLog, true)) {
    return false;
  }

  // Initialize log file
  AppendToLog(F("Data Log Initialing - "), false);
  AppendToLog(fileName, true);

  // TODO: Write timestamp (UNIX)

  return true;
}

// Append data to the current .log file
void AppendToLog(char *logInput, bool endLine) {
  SDWriteStream(fileLocationLog, true);
  SDWriteStream(logInput);
  if (endLine) SDWriteStreamNewLine();
  SDQuit();
}

// Convert String input to char* and append to .log
void AppendToLog(String logInput, bool endLine) {
  char *logInputChar = &logInput[0];
  AppendToLog(logInputChar, endLine);
}

// Append to .log without lineEnd
void AppendToLog(String logInput) {
  AppendToLog(logInput, false);
}

// Append to .log without lineEnd
void AppendToLog(char *logInput) {
  AppendToLog(logInput, false);
}

// Append data to the current data file
void AppendToData(char *dataInput, bool endLine) {
  SDWriteStream(fileLocationData, true);
  SDWriteStream(dataInput);
  if (endLine) SDWriteStreamNewLine();
  SDQuit();
}

void AppendToData(char *dataInput) {
  AppendToData(dataInput, false);
}
