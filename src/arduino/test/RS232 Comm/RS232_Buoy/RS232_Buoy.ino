/*
  Lake Profiler Buoy RS232 Test Script

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "./config/_pinout.h"

const String testCMD = "Hello World from Buoy\r\n";
unsigned long lastMillisSent;
bool LedState;

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing RS232... "));

  // Enable Canister Power
  pinMode(PO_POWER_CANISTER, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PO_POWER_CANISTER, true);

  if (RS232Initialize()) {
    Serial.println(F("Success"));
  } else {
    Serial.println("Error");
    while (1) {
    }
  }
}

void loop() {
  if (COM_SERIAL_CANISTER.available() > 0) {
    char recChar = COM_SERIAL_CANISTER.read();

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
      COM_SERIAL_CANISTER.print("1\r\n");
    } else {
      COM_SERIAL_CANISTER.print("0\r\n");
    }
  }
}

bool RS232Initialize() {
  COM_SERIAL_CANISTER.begin(9600);
  return COM_SERIAL_CANISTER;
}

bool CH4Initialize() {
  return true;
}