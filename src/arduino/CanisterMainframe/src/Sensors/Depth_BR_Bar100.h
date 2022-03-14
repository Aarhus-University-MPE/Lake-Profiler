/*
    Depth sensor (Blue Robotics Bar100), handles reading sensor output.

    https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar100-sensor-r2-rp/

    https://github.com/bluerobotics/BlueRobotics_KellerLD_Library

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Arduino.h"
#include "KellerLD.h"

KellerLD bar100Sensor;

bool DepthInitialize();

void DepthTerminate();

bool DepthStatus();

bool DepthTest();

void DepthRead();