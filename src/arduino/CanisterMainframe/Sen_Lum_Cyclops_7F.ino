/*
    Luminesence sensor (Cyclops-7F), handles reading sensor output.

    https://www.turnerdesigns.com/cyclops-7f-submersible-fluorometer

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/
int lumValue;

bool LumInitialize() {
  return true;
}

void LumTerminate() {
}

bool LumStatus() {
  return GetStatus(MODULE_LUM);
}

bool LumTest() {
  return true;
}

// Read Sensor Data
void LumRead() {
  lumValue = analogRead(PA_LUM_SENSOR);
}

int LumGetValue() {
  return lumValue;
}
