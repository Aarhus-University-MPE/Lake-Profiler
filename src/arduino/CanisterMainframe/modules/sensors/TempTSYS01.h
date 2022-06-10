/*
    Temp sensor (Blue Robotics TSYS01), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/celsius-sensor-r1/

    https://github.com/bluerobotics/BlueRobotics_TSYS01_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#pragma once
#include "../setup/modules.h"

// Library:
#include "TSYS01.h"

TSYS01 sensorTSY01;
uint8_t tempData;

bool TempInitialize() {
  sensorTSY01.init();

  return GetTemp() != 0;
}

void TempTerminate() {
}

bool TempStatus() {
  return GetStatus(MODULE_TEMP);
}

bool TempTest() {
  sensorTSY01.read();

  DEBUG_PRINT(F("Temperature: "));

  DEBUG_PRINT(sensorTSY01.temperature());

  DEBUG_PRINTLN(F(" deg C"));
  return true;
}

void TempRead() {
  sensorTSY01.read();
}

float GetTemp() {
  TempRead();
  return sensorTSY01.temperature();
}

bool TempSendPackage() {
  union unpack pack;
  uint8_t package[5];

  // Add Temperature Wrapper
  package[0] = 'T';

  // Convert Temperature float to binary array
  pack.f = GetTemp();
  for (int i = 0; i < 4; i++) {
    package[i + 1] = pack.b[i];
  }

  // Send Package over RS232
  return BuoySendPackage(package, 5);
}