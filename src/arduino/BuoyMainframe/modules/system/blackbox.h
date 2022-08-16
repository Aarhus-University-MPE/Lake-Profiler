/*
  Lake Profiler Blackbox protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#pragma once

#include "../setup/modules.h"

#define MAX_BLACKBOX_ROW_PRINT 100

unsigned long lastMillisHeartbeatBlackbox = 0;

bool newLine = true;

// Initialize system Blackbox
bool InitBlackBox() {
  if (!SDReaderStatus()) {
    return false;
  }

  bool status = SD.open("Blackbox.csv", FILE_WRITE);
  if (status) {
    BlackBoxAppend("Lake Profiler Blackbox ");
    BlackBoxAppend("System Version: ");
    BlackBoxAppendln(SystemVersion);
  }

  return status;
}

void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > HRTBEAT_DT_LOG) {
    lastMillisHeartbeatBlackbox = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Heartbeat System Check: "));
    SystemCheck();
  }
}

/*
  Prints Contents of BlackBox File
  TODO: print newest lines only, currently prints first lines
*/
void BlackBoxPrint() {
  if (!SDReaderStatus()) {
    return;
  }
  DEBUG_PRINTLN(F("Printing Blackbox: "));
  File file = SD.open("Blackbox.csv");
  if (file) {
    int idx = 0;
    char c;
    while (file.available() && idx < MAX_BLACKBOX_ROW_PRINT) {
      c = file.read();
      Serial.write(c);
      if (c == '\n') idx++;
    }
    file.close();
    if (idx < MAX_BLACKBOX_ROW_PRINT) {
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("End of File"));
    } else {
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Printed 100 Rows"));
    }
  } else {
    DEBUG_PRINTLN(F("File not found!"));
  }
}

void BlackBoxEmpty() {
  if (!BlackBoxStatus()) return;

  if (SD.exists("Blackbox.csv")) {
    DEBUG_PRINTLN(F("BLACKBOX CLEARED"));
    SD.remove("Blackbox.csv");
    InitBlackBox();
  } else {
    SetStatus(MODULE_BLACKBOX, false);
    DEBUG_PRINTLN(F("BLACKBOX ERROR"));
  }
}

bool BlackBoxStatus() {
  return GetStatus(MODULE_BLACKBOX);
}

void BlackBoxAppend(String blackBoxInput) {
  if (!BlackBoxStatus()) return;

  File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
  if (blackBoxFile) {
    if (newLine) {
      blackBoxFile.print((String)now());
      blackBoxFile.print(F(";"));
      newLine = false;
    }
    blackBoxFile.print(blackBoxInput);
    blackBoxFile.close();
  } else {
    SetStatus(MODULE_BLACKBOX, false);
    DEBUG_PRINTLN(F("Blackbox Error"));
  }
}

void BlackBoxAppendln() {
  BlackBoxAppend("\r\n");
  newLine = true;
}

void BlackBoxAppendln(String blackBoxInput) {
  BlackBoxAppend(blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(long int blackBoxInput, int type) {
  if (!BlackBoxStatus()) return;

  File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
  if (blackBoxFile) {
    if (newLine) {
      blackBoxFile.print((String)now());
      blackBoxFile.print(F(";"));
      newLine = false;
    }
    blackBoxFile.print(blackBoxInput, type);
    blackBoxFile.close();
  } else {
    SetStatus(MODULE_BLACKBOX, false);
    DEBUG_PRINTLN(F("Blackbox Error"));
  }
}

void BlackBoxAppendln(long int blackBoxInput, int type) {
  BlackBoxAppend(blackBoxInput, type);
  BlackBoxAppendln();
}

void BlackBoxAppendln(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppend(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}
