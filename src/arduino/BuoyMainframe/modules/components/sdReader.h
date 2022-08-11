#pragma once
#include "../setup/modules.h"

/*
  Lake Profiler local storage protocols

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include <SD.h>

File activeWriteFile;

// Initialize SD card reader module.
bool InitializeSDReader() {
  bool status = false;
  if (SD.begin(PO_SPISS_SDCARD)) {
    status = SD.mkdir("data");
  }
  return status;
}

void TerminateSDReader() {
  SD.end();
}
// Checks status of SD reader
bool SDReaderStatus() {
  return GetStatus(MODULE_SD);
}

// Query all files on SD card and print to serial prompt
void SDQuery() {
  if (SDReaderStatus()) {
    File dir = SD.open("/");
    dir.rewindDirectory();
    printDirectory(dir, 0);
    dir.rewindDirectory();
    dir.close();
    delay(10);
  }
}

// Print full directory
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();

    if (!entry) {
      // no more files
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      DEBUG_PRINT('\t');
    }

    DEBUG_PRINT(entry.name());

    if (entry.isDirectory()) {
      DEBUG_PRINTLN("/");

      printDirectory(entry, numTabs + 1);

    } else {
      // files have sizes, directories do not

      DEBUG_PRINT("\t\t");

      DEBUG_PRINT2(entry.size(), DEC);
      DEBUG_PRINTLN();
    }

    entry.close();
  }
}
// Print all files to serial port
void printFiles(File dir) {
  DEBUG_PRINTLN(F(("Files in system:")));
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      File entry = dir.openNextFile();
      if (!entry)
        break;
    }

    if (!entry.isDirectory()) {
      DEBUG_PRINT(entry.name());
      DEBUG_PRINT(F(("\t\t")));
      DEBUG_PRINTLN2(entry.size(), DEC);
    }
    entry.close();
  }
  DEBUG_PRINTLN(F(("End of storage")));
  DEBUG_PRINTLINE();
}

// Print size of file on SD card
void SDSize(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINT(F("Opening file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      DEBUG_PRINTLN("File size: " + (String)file.size() + " bytes");
      file.close();
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Print all data from datafile to serial port
void SDDownload(char fileName[]) {
  if (SDReaderStatus()) {
    // appendCsv(fileName);
    DEBUG_PRINTLN(F("Downloading file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      while (file.available()) {
        uint8_t data = file.read();
        Serial.print(data, HEX);
        Serial.print(F(" "));
        if (data == '\r') {
          uint8_t data2 = file.read();
          if (data2 == '\n') {
            Serial.println();
          } else {
            Serial.print(data2, HEX);
            Serial.print(F(" "));
          }
        }
      }
      file.close();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("End of File"));
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Delete file on SD card
void SDDelete(char fileName[]) {
  if (SDReaderStatus()) {
    // appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINT(F("Deleting file: "));
      DEBUG_PRINT(fileName);
      SD.remove(fileName);
      DEBUG_PRINTLN(F(" - File Removed"));
    } else {
      DEBUG_PRINTLN(F(" - File not found"));
    }
  }
}

// Create empty file on SD card
bool SDCreate(char fileName[]) {
  if (!SDReaderStatus()) {
    DEBUG_PRINTLN(F("SD Reader Error"));
    return false;
  }
  appendCsv(fileName);
  if (SD.exists(fileName)) {
    SDDelete(fileName);
    SDCreate(fileName, true);
  } else {
    DEBUG_PRINT(F("Creating file: "));
    DEBUG_PRINT(fileName);
    File file = SD.open(fileName, FILE_WRITE);
    if (file) {
      DEBUG_PRINTLN(F(" - Success!"));
      file.close();
      return true;
    } else {
      DEBUG_PRINTLN(F(" - Error!"));
      return false;
    }
  }
}

// Create empty file on SD card with custom fileending
bool SDCreate(char fileName[], bool customFileEnd) {
  if (!SDReaderStatus()) {
    DEBUG_PRINTLN(F("SD Reader Error"));
    return false;
  }
  if (!customFileEnd) {
    return SDCreate(fileName);
  }
  if (SD.exists(fileName)) {
    SDDelete(fileName);
    SDCreate(fileName, true);
  } else {
    DEBUG_PRINT(F("Creating file: "));
    DEBUG_PRINT(fileName);
    File file = SD.open(fileName, FILE_WRITE);
    if (file) {
      DEBUG_PRINTLN(F(" - Success!"));
      file.close();
      return true;
    } else {
      DEBUG_PRINTLN(F(" - Error!"));
      return false;
    }
  }
}

/*
  Initializes file write stream
  First run opens file with name, consecutive runs appends data to opened file
  When complete run SDQuit() to close write stream
*/
bool SDWriteStream(char fileNameOrData[], bool customFileEnd) {
  if (!SDReaderStatus()) {
    DEBUG_PRINTLN(F("SD Reader Error"));
    return false;
  }
  if (!activeWriteFile) {
    return SDOpenWriteStream(fileNameOrData, customFileEnd);
  }

  // DEBUG_PRINT(F("Writing to file: "));
  // DEBUG_PRINTLN(fileNameOrData);

  activeWriteFile.write(fileNameOrData);
  return true;
}

/*
  Initializes file write stream
  First run opens file with name, consecutive runs appends data to opened file
  When complete run SDQuit() to close write stream
*/
bool SDWriteStream(char fileNameOrData[]) {
  SDWriteStream(fileNameOrData, false);
  return true;
}

/*
  Write binary data to file stream
  When complete run SDQuit() to close write stream
*/
bool SDWriteStream(uint8_t data[], uint8_t size, bool customFileEnd) {
  if (!SDReaderStatus()) {
    DEBUG_PRINTLN(F("SD Reader Error"));
    return false;
  }
  if (!activeWriteFile) {
    DEBUG_PRINTLN(F("No active file"));
    return false;
  }

  // DEBUG_PRINT(F("Writing to file: "));
  // DEBUG_PRINTLN(data);

  activeWriteFile.write(data, size);
  return true;
}

/*
  Write binary data to file stream
  When complete run SDQuit() to close write stream
*/
bool SDWriteStream(uint8_t data[], uint8_t size) {
  SDWriteStream(data, size, false);
  return true;
}

/*
  Initializes file write stream
  First run opens file with name, consecutive runs appends data to opened file
  When complete run SDQuit() to close write stream
*/
bool SDWriteStreamNewLine() {
  if (!activeWriteFile) {
    DEBUG_PRINTLN(F("No active file!"));
    return false;
  }

  // DEBUG_PRINTLN(F("Writing newline char to file"));

  activeWriteFile.write('\r');
  activeWriteFile.write('\n');

  return true;
}

bool SDOpenWriteStream(char fileName[], bool customEndLine) {
  if (!customEndLine) {
    appendCsv(fileName);
  }

  // TODO: remove?
  if (!SD.exists(fileName)) {
    DEBUG_PRINT(F("Creating file: "));
    DEBUG_PRINT(fileName);
    activeWriteFile = SD.open(fileName, FILE_WRITE);
    if (!activeWriteFile) {
      DEBUG_PRINTLN(F(" - Error!"));
    } else {
      DEBUG_PRINTLN(F(" - Success"));
    }
  } else {
    activeWriteFile = SD.open(fileName, FILE_WRITE);
  }

  return activeWriteFile;
}

// Closes current active file write stream
void SDQuit() {
  // DEBUG_PRINTLN(F("Closing active file"));
  activeWriteFile.close();
}
