/*
    Depth sensor (Blue Robotics Bar100), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar100-sensor-r2-rp/

    https://github.com/bluerobotics/BlueRobotics_KellerLD_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Depth_BR_Bar100.h"

bool DepthInitialize() {
  bar100Sensor.init();
  bar100Sensor.setFluidDensity(997);  // kg/m^3 (freshwater, 1029 for seawater)

  return bar100Sensor.isInitialized();
}

void DepthTerminate() {
}

bool DepthStatus() {
  return GetStatus(MODULE_DEPTH);
}

bool DepthTest() {
  return true;
}

void DepthRead() {
  bar100Sensor.read();

  DEBUG_PRINT("Pressure: ");
  DEBUG_PRINT(bar100Sensor.pressure());
  DEBUG_PRINTLN(" mbar");

  DEBUG_PRINT("Temperature: ");
  DEBUG_PRINT(bar100Sensor.temperature());
  DEBUG_PRINTLN(" deg C");

  DEBUG_PRINT("Depth: ");
  DEBUG_PRINT(bar100Sensor.depth());
  DEBUG_PRINTLN(" m");

  DEBUG_PRINT("Altitude: ");
  DEBUG_PRINT(bar100Sensor.altitude());
  DEBUG_PRINTLN(" m above mean sea level");
}