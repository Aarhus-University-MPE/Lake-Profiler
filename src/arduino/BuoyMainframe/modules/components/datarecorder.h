/*  Autonomous Process

    Primary processes for autonomous modes
*/
#pragma once
#include "../setup/modules.h"

char fileLocationData[22];
char fileLocationLog[22];
File logFile;
File dataFile;

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
  TimeStampLog();

  DEBUG_PRINT(F("Current time: "));
  DEBUG_PRINTLN(now());

  return true;
}

// Append data to the current .log file
void AppendToLog(char *logInput, bool endLine) {
  if (!SDReaderStatus()) return;
  if (!LoggingActive()) return;
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
  if (!SDReaderStatus()) return;
  if (!LoggingActive()) return;
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
  if (!SDReaderStatus()) return;
  if (!LoggingActive()) return;
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

// TODO: load latest log file
bool LogFileLoad() {
  // Load Log file
  logFile = SD.open(fileLocationLog);

  // Check first line
  if (logFile) {
    return true;
  } else {
    DEBUG_PRINTLN(F("File not found!"));
    return false;
  }
}

// TODO: load latest data file
bool DataFileLoad() {
  // Load Data file
  dataFile = SD.open(fileLocationData);

  if (dataFile) {
    return true;
  } else {
    DEBUG_PRINTLN(F("File not found!"));
    return false;
  }
}

bool LogReadLine(uint8_t *package, int &size) {
  // read data
  if (!SDReaderStatus()) return true;

  if (!logFile) return true;

  size         = 0;
  bool endLine = false;

  while (logFile.available() && !endLine) {
    uint8_t data  = logFile.read();
    package[size] = data;
    size++;
    if (data == '\n') endLine = true;
  }
  if (!logFile.available()) {
    return true;
    DEBUG_PRINTLN(F("End of File"));
    logFile.close();
  }

  return false;
}

bool DataReadLine(uint8_t *package, int &size) {
  // read data
  if (!SDReaderStatus()) return true;

  if (!dataFile) return true;

  size         = 0;
  bool endLine = false;

  while (dataFile.available() && !endLine) {
    uint8_t data  = dataFile.read();
    package[size] = data;
    DEBUG_PRINT2(data, HEX);
    DEBUG_PRINT(F(" "));
    size++;
    if (data == '\n') {
      endLine = true;
      DEBUG_PRINTLN();
    }
  }
  if (!dataFile.available()) {
    return true;
    DEBUG_PRINTLN(F("End of File"));
    dataFile.close();
  }

  return false;
}