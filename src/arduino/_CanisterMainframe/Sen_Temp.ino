/*
    Temp sensor (Blue Robotics TSY01), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/celsius-sensor-r1/

    https://github.com/bluerobotics/BlueRobotics_TSYS01_Library
    
    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "TSY01.h"

TSYS01 sensorTSY01;

bool TempInitialize(){

  sensorTSY01.init();

  return true;
}


void TempTerminate(){

}

void TempRead(){
  sensorTSY01.read();

  DEBUG_PRINT("Temperature: ");
  
  DEBUG_PRINT(sensor.temperature()); 
  
  DEBUG_PRINTLN(" deg C");
}