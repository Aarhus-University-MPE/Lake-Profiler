/*
    Depth sensor (Blue Robotics Bar100), handles reading sensor output.

    Component:
      https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar100-sensor-r2-rp/

    Primary Library:
      https://github.com/bluerobotics/BlueRobotics_KellerLD_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#pragma once
#include "../setup/modules.h"

// Library:
#include "KellerLD.h"
KellerLD bar100Sensor;

bool DepthInitialize() {
  bar100Sensor.init();
  bar100Sensor.setFluidDensity(997);  // kg/m^3 (freshwater, 1029 for seawater)

  // return true;  // TODO: Remove
  return bar100Sensor.isInitialized();
}

void DepthTerminate() {
}

bool DepthStatus() {
  return GetStatus(MODULE_DEPTH);
}

bool DepthTest() {
  bar100Sensor.read();

  DEBUG_PRINT(F("Pressure: "));
  DEBUG_PRINT(bar100Sensor.pressure());
  DEBUG_PRINTLN(F(" mbar"));

  DEBUG_PRINT(F("Temperature: "));
  DEBUG_PRINT(bar100Sensor.temperature());
  DEBUG_PRINTLN(F(" deg C"));

  DEBUG_PRINT(F("Depth: "));
  DEBUG_PRINT(bar100Sensor.depth());
  DEBUG_PRINTLN(F(" m"));

  DEBUG_PRINT(F("Altitude: "));
  DEBUG_PRINT(bar100Sensor.altitude());
  DEBUG_PRINTLN(F(" m above mean sea level"));

  return true;
}

void DepthRead() {
  bar100Sensor.read();
}

float GetDepth() {
  bar100Sensor.read();
  return 3.14f;  // TODO: remove
  return bar100Sensor.depth();
}

bool DepthSendPackage() {
  union unpack pack;
  uint8_t package[5];

  // Add Depth Wrapper
  package[0] = 'D';

  // Convert Depth float to binary array
  pack.f = GetDepth();
  for (int i = 0; i < 4; i++) {
    package[i + 1] = pack.b[i];
  }

  // Send Package over RS232
  return BuoySendPackage(package, 5);
}
