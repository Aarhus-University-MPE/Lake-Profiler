/*  Autonomous Process

    Primary processes for autonomous modes
*/
#pragma once
#include "../setup/modules.h"

char fileLocationData[22];
char fileLocationLog[22];
unsigned long dataIndex = 0;
unsigned int sampleID   = 0;
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

  // Initialize dataIndex
  dataIndex = 0;
  sampleID  = IncrementSampleID();

  DEBUG_PRINT(F("Sample id: "));
  DEBUG_PRINTLN((int)sampleID);

  return true;
}

void SetSampleID(unsigned int idValue) {
  EEPROM_WRITE_UINT(MEMADDR_SAMPLEID, idValue);
}

unsigned int GetSampleID() {
  return EEPROM_READ_UINT(MEMADDR_SAMPLEID);
}

unsigned int IncrementSampleID() {
  SetSampleID(GetSampleID() + 1);
  return GetSampleID();
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

// Append data without lineend
void AppendToData(uint8_t *dataInput, uint8_t size) {
  AppendToData(dataInput, size, false);
}

// Append Package Identifier
void AppendPackageIdentifierToData() {
  uint8_t identifierData[1];
  identifierData[0] = 'A';

  AppendToData(identifierData, 1, false);
}

// Append and incre
void AppendIndexToData() {
  uint8_t indexData[4];
  // Convert index to binary
  for (uint8_t i = 0; i < 4; i++) {
    indexData[i] = ((dataIndex >> (8 * i)) & 0XFF);
  }

  dataIndex++;
  AppendToData(indexData, 4, false);
}

// Return total number of data lines
unsigned long GetDataLines() {
  return dataIndex;
}

// Append sample ID to data row
void IdStampData() {
  union unpack pack;

  pack.ui = sampleID;
  AppendToData(pack.b, 2, false);
}

// Add current timestamp
void TimeStampData() {
  uint8_t timeStampData[4];
  unsigned long timeStamp = (unsigned long)now();

  // Convert timestamp to binary
  for (uint8_t i = 0; i < 4; i++) {
    timeStampData[i] = ((timeStamp >> (8 * i)) & 0XFF);
  }

  // DEBUG_PRINT(F("Current time: "));
  // DEBUG_PRINTLN(timeStamp);
  AppendToData(timeStampData, 4, true);
}

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

// Read single line from data file
bool DataReadLine(uint8_t *package, int &size) {
  // read data
  if (!SDReaderStatus()) return true;

  if (!dataFile) return true;

  size         = 0;
  bool endLine = false;

  while (dataFile.available() && !endLine) {
    uint8_t data = dataFile.read();
    if (data == '\r') {
      uint8_t data2 = dataFile.read();
      if (data2 == '\n') {
        // DEBUG_PRINTLN();
        endLine = true;
      } else {
        package[size] = data2;
        size++;
      }
    }
    if (!endLine) {
      package[size] = data;
      size++;
      // DEBUG_PRINT2(data, HEX);
      // DEBUG_PRINT(F(" "));
    }
  }
  if (!dataFile.available()) {
    return true;
    DEBUG_PRINTLN(F("End of File"));
    dataFile.close();
  }

  return false;
}

bool LogReadLine(uint8_t *package, int &size) {
  // read data
  if (!SDReaderStatus()) return true;

  if (!logFile) return true;

  size         = 0;
  bool endLine = false;

  while (logFile.available() && !endLine) {
    uint8_t data = logFile.read();
    if (data == '\r') {
      endLine = true;
    } else {
      package[size] = data;
      size++;
    }
  }
  if (!logFile.available()) {
    return true;
    DEBUG_PRINTLN(F("End of File"));
    logFile.close();
  }

  return false;
}