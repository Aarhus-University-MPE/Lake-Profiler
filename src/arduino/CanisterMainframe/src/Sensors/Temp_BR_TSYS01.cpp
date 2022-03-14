/*
    Temp sensor (Blue Robotics TSYS01), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/celsius-sensor-r1/

    https://github.com/bluerobotics/BlueRobotics_TSYS01_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Temp_BR_TSYS01.h"

bool TempInitialize() {
  sensorTSY01.init();

  return true;
}

void TempTerminate() {
}

bool TempStatus() {
  return GetStatus(MODULE_TEMP);
}

bool TempTest() {
  return true;
}

void TempRead() {
  sensorTSY01.read();

  DEBUG_PRINT(F("Temperature: "));

  DEBUG_PRINT(sensorTSY01.temperature());

  DEBUG_PRINTLN(F(" deg C"));
}
