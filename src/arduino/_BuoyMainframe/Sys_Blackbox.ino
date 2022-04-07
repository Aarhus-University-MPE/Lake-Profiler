/*
  Lake Profiler Blackbox protocols

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

bool newLine = true;
// Initialize system Blackbox
bool InitBlackBox() {
  bool status = true;

  if (!InitializeSDReader()) {
    status = false;
    return status;
  }

  if (SDReaderStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      DEBUG_PRINTLN(F("Initializing Blackbox"));
      blackBoxFile.println(F("GeoRover Blackbox"));
      blackBoxFile.print(F("System Version:;"));
      blackBoxFile.println(SystemVersion);
      blackBoxFile.println();
      blackBoxFile.close();

    } else
      SetStatus(MODULE_BLACKBOX, false);
    status = false;
  } else {
    DEBUG_PRINTLN(F("SD connection error!"));
    SetStatus(MODULE_BLACKBOX, false);
    status = false;
  }
  return status;
}

bool BlackBoxStatus() {
  return GetStatus(MODULE_BLACKBOX);
}

unsigned long lastMillisHeartbeatBlackbox = 0;
void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > HRTBEAT_DT_LOG) {
    lastMillisHeartbeatBlackbox = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINT(F("Heartbeat System Status: "));
    DEBUG_PRINTLN(String(ToLong(SystemStatus)));
    DEBUG_PRINTLINE();
  }
}

void BlackBoxAppendln() {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(F(";"));
      }
      blackBoxFile.println();
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN(F("Blackbox Error"));

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(float blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(byte blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}
void BlackBoxAppendln(char blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(bool blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput, int Type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.print(blackBoxInput, Type);
      blackBoxFile.println();
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(float blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(byte blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(char blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(bool blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(long int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(long int blackBoxInput, int Type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput, Type);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}
