/*
    CH4 sensor (Contros HydroC CH4), handles reading sensor output.

    https://www.4h-jena.de/en/maritime-technologies/sensors/hydrocrch4/

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Arduino.h"

bool CH4Initialize();

void CH4Terminate();

bool CH4Status();

bool CH4Test();

void CH4Read();