/*
    Luminesence sensor (Cyclops-7F), handles reading sensor output.

    https://www.turnerdesigns.com/cyclops-7f-submersible-fluorometer

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/
int lumValue = 314;

bool LumInitialize() {
  // pinMode(PA_LUM_SENSOR, INPUT);
  return true;
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
