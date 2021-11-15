/*
    Depth sensor (Blue Robotics Bar100), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar100-sensor-r2-rp/

    https://github.com/bluerobotics/BlueRobotics_KellerLD_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "KellerLD.h"

KellerLD bar100Sensor;


bool DepthInitialize(){
  bar100Sensor.init();
  bar100Sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

  return bar100Sensor.isInitialized();
}


void DepthTerminate(){

}

void DepthRead(){
  bar100Sensor.read();
  
  DEBUG_PRINT("Pressure: "); 
  DEBUG_PRINT(sensor.pressure()); 
  DEBUG_PRINTLN(" mbar");
  
  DEBUG_PRINT("Temperature: "); 
  DEBUG_PRINT(sensor.temperature()); 
  DEBUG_PRINTLN(" deg C");
  
  DEBUG_PRINT("Depth: "); 
  DEBUG_PRINT(sensor.depth()); 
  DEBUG_PRINTLN(" m");
  
  DEBUG_PRINT("Altitude: "); 
  DEBUG_PRINT(sensor.altitude()); 
  DEBUG_PRINTLN(" m above mean sea level");
}