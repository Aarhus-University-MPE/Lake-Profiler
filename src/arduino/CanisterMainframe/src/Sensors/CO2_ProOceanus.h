/*
    CO2 sensor (Pro Oceanus Mini CO2), handles reading sensor output.

    https://pro-oceanus.com/products/mini-series/mini-co2

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Arduino.h"

bool CO2Initialize();

void CO2Terminate();


bool CO2Status();

bool CO2Test();

void CO2Read();