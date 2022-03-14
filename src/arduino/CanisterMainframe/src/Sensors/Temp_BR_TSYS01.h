/*
    Temp sensor (Blue Robotics TSYS01), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/celsius-sensor-r1/

    https://github.com/bluerobotics/BlueRobotics_TSYS01_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Arduino.h"
#include "TSYS01.h"

TSYS01 sensorTSY01;

bool TempInitialize();

void TempTerminate();

bool TempStatus();

bool TempTest();

void TempRead();
