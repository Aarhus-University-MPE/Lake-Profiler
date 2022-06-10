/*
    Luminesence sensor (Cyclops-7F), handles reading sensor output.

    https://www.turnerdesigns.com/cyclops-7f-submersible-fluorometer

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/

#pragma once
#include "../setup/modules.h"

int lumValue = 314;

bool LumInitialize() {
  pinMode(PA_LUM_SENSOR, INPUT);

  return GetLumValue() != 0;
}

void LumTerminate() {
}

bool LumStatus() {
  return GetStatus(MODULE_LUM);
}

bool LumTest() {
  DEBUG_PRINT(F("Luminesence Reading: "));
  DEBUG_PRINTLN(GetLumValue());
  return true;
}

// Read Sensor Data
void LumRead() {
  lumValue = analogRead(PA_LUM_SENSOR);
}

int GetLumValue() {
  LumRead();
  return lumValue;
}

bool LumSendPackage() {
  union unpack pack;
  uint8_t package[5];

  // Add Luminesence Wrapper
  package[0] = 'L';

  // Convert Luminesence int to binary array
  pack.f = GetLumValue();
  for (int i = 0; i < 2; i++) {
    package[i + 1] = pack.b[i];
  }

  // Send Package over RS232
  return BuoySendPackage(package, 3);
}
