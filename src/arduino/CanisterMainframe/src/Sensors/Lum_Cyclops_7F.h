/*
    Luminesence sensor (Cyclops-7F), handles reading sensor output.

    https://www.turnerdesigns.com/cyclops-7f-submersible-fluorometer

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#include "Arduino.h"

bool LumInitialize();

void LumTerminate();

bool LumStatus();

bool LumTest();

void LumRead();