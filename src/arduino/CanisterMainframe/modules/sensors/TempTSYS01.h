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

long GetTemp() {
  TempRead();
  float temp = sensorTSY01.temperature();

  long longTemp = (long)(temp * 1000.0f);

  return longTemp;
}
