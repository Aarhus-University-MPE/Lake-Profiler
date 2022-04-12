/*
  Lake Profiler Canister RS232 Test Script

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "./config/_pinout.h"

const String testCMD = "Hello World from Canister\r\n";
unsigned long lastMillisSent;
bool LedState;

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing RS232... "));

  pinMode(LED_BUILTIN, OUTPUT);

  if (RS232Initialize()) {
    Serial.println(F("Success"));
  } else {
    Serial.println("Error");
    while (1) {
    }
  }
}

void loop() {
  if (COM_SERIAL_BUOY.available() > 0) {
    char recChar = COM_SERIAL_BUOY.read();

    Serial.write(recChar);
    switch (recChar) {
      case '0':
        digitalWrite(LED_BUILTIN, false);
        break;
      case '1':
        digitalWrite(LED_BUILTIN, true);
        break;
      default:
        break;
    }
  }

  if (millis() - lastMillisSent > 1000) {
    lastMillisSent = millis();
    LedState       = !LedState;
    if (LedState) {
      COM_SERIAL_BUOY.print("1\r\n");
    } else {
      COM_SERIAL_BUOY.print("0\r\n");
    }
  }
}

bool RS232Initialize() {
  COM_SERIAL_BUOY.begin(9600);
  return COM_SERIAL_BUOY;
}

bool CH4Initialize() {
  return true;
}