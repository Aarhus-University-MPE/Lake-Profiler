
/*
  Lake Profiler Power control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once
#include <LowPower.h>

#include "../setup/modules.h"

// BatteryReading = 3 kOhm / (3 kOhm + 9 kOhm) * Battery Voltage
// 13V .. 11.7V -> ~3.25V .. 2.93V -> 665 .. 599
const float batteryScale  = 69.23;
const float batteryOffset = -800.0f;

bool charge;  // Battery Charge status

int BatteryLevel() {
  float batteryLevel = batteryScale * BatteryVoltage() + batteryOffset;

  return max((int)batteryLevel, 0);
}

// Checks voltage levels above critical values?
bool VoltageCheck() {
  return BatteryLevel() > BATTERY_MIN_LEVEL;
}

float BatteryVoltage() {
  int voltageInt = analogRead(PA_SENSOR_BATT);

  float voltageRead = voltageInt * 5.0f / 1024.0f;

  float voltageBattery = voltageRead * 12.0f / 3.0f;

  return voltageBattery;
}

bool BatteryStatus() {
  return VoltageCheck();
}

bool BatteryStatus(bool print) {
  if (!print) {
    return VoltageCheck();
  }

  DEBUG_PRINT(F("Battery Level: "));
  DEBUG_PRINTLN(BatteryLevel());

  DEBUG_PRINT(F("Battery Voltage: "));
  DEBUG_PRINTLN(BatteryVoltage());
  return VoltageCheck();
}

void StandbyMode() {
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void wakeUp() {
  // Just a handler for the pin interrupt.
}
