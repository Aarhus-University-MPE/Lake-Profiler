/*
    CH4 sensor (Contros HydroC CH4), handles reading sensor output.

    https://www.4h-jena.de/en/maritime-technologies/sensors/hydrocrch4/

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "CH4_Contros_HydroC.h"

bool CH4Initialize() {
  return true;
}

void CH4Terminate() {
}

bool CH4Status() {
  return GetStatus(MODULE_CH4);
}

bool CH4Test() {
  return true;
}

void CH4Read() {
}