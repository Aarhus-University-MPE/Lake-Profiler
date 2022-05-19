/*  Autonomous Process

    Primary processes for autonomous modes
*/
#pragma once
#include "../setup/modules.h"

char fileLocationData[22];
char fileLocationLog[22];

// Create logging files (YYMMDDHH.log and YYMMDDHH.csv)
bool InitializeLoggingFiles() {
  char folder[8]   = "/data/";
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

  // Append .log
  strcat(fileLocationLog, fileLocationData);
  strcat(fileLocationLog, fileEnd);

  // Create data file
  if (!SDCreate(fileLocationData)) return false;
  DEBUG_PRINT(F("data file: "));
  DEBUG_PRINTLN(fileLocationData);

  // Create log file
  DEBUG_PRINT(F("log file: "));
  DEBUG_PRINTLN(fileLocationLog);
  if (!SDCreate(fileLocationLog, true)) return false;

  // Initialize log file
  AppendToLog(F("Data Log Initialing - "), false);
  AppendToLog(fileName, true);
  AppendToLog(F("Timestamp: "));
  AppendToLog((String)now(), true);

  DEBUG_PRINT(F("Current time: "));
  DEBUG_PRINTLN(now());

  return true;
}

// Append data to the current .log file
void AppendToLog(char *logInput, bool endLine) {
  if (!SDReaderStatus()) return;
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

// Append dat without lineend
void AppendToData(char *dataInput) {
  AppendToData(dataInput, false);
}

// Append data to the current data file
void AppendToData(uint8_t *dataInput, uint8_t size, bool endLine) {
  SDWriteStream(fileLocationData, true);
  SDWriteStream(dataInput, size);
  if (endLine) SDWriteStreamNewLine();
  SDQuit();
}

// Append dat without lineend
void AppendToData(uint8_t *dataInput, uint8_t size) {
  AppendToData(dataInput, size, false);
}

// Add current timestamp
void TimeStampData() {
  uint8_t timeStampData[4];
  unsigned long timeStamp = (unsigned long)now();

  for (uint8_t i = 0; i < 4; i++) {
    timeStampData[i] = ((timeStamp >> (8 * i)) & 0XFF);
  }

  DEBUG_PRINT(F("Current time: "));
  DEBUG_PRINTLN(timeStamp);

  AppendToData(timeStampData, 4, true);
}