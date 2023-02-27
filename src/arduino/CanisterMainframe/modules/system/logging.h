/*
  Lake Profiler Canister Logging functionalities

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#pragma once
#include "../setup/modules.h"

unsigned int loggingSampleInterval;
unsigned long lastMillisSensor = 0;
uint8_t packageSize            = 28;
uint8_t packageIndex           = 0;
uint8_t package[28];

// Send all packages
bool SendPackage() {
  DEBUG_PRINTLN(F("Sending Package"));
  DEBUG_PRINTLINE();

  BuildPackage();
  if (!BuoySendPackage(package, packageSize)) return false;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);

  // if (!SendFooter()) return false;
  // DEBUG_PRINTLINE();
  return true;
}

// Sensor Package Broadcast
void SensorBroadcast() {
  if (millis() - lastMillisSensor < loggingSampleInterval) return;

  lastMillisSensor = millis();

  // Rebuild and send Package with latest data
  SendPackage();
}

// Primary Sensor Loop
void SensorProcess() {
  // Read sensor data
  SensorRead();

  // Send data
  SensorBroadcast();
}

// Process all incoming sensors data
void SensorRead() {
  CH4Read();
  CO2Read();
  // DepthRead();
  // LumRead(); // Analog sensor, no need to constantly update
  // TempRead();
}

// Append value to package
void AppendFloat(float data) {
  union unpack pack;
  pack.f = data;

  for (int i = 0; i < 4; i++) {
    package[packageIndex + i] = pack.b[i];
  }
  packageIndex += 4;
}

// Append value to package
void AppendInt(int data) {
  union unpack pack;
  pack.i = data;

  package[packageIndex]     = pack.b[0];
  package[packageIndex + 1] = pack.b[1];

  packageIndex += 2;
}

// Append value to package
void AppendLong(long data) {
  union unpack pack;
  pack.l = data;

  package[packageIndex]     = pack.b[0];
  package[packageIndex + 1] = pack.b[1];
  package[packageIndex + 2] = pack.b[2];
  package[packageIndex + 3] = pack.b[3];

  packageIndex += 4;
}

// Append value to package
void AppendUnsignedInt(unsigned int data) {
  union unpack pack;
  pack.ui = data;

  package[packageIndex]     = pack.b[0];
  package[packageIndex + 1] = pack.b[1];

  packageIndex += 2;
}

// Clear package and add package header
void InitializePackage() {
  memset(package, 0, packageSize);
  package[0]   = PACKAGE_0;
  packageIndex = 1;
}

void PrintData() {
  DEBUG_PRINT(F("Package Data - CH4: "));
  DEBUG_PRINT(GetCH4ConcentrationEstimate());
  DEBUG_PRINT(F("\t"));
  DEBUG_PRINT(GetCH4Concentration());
  DEBUG_PRINT(F("\t CO2: "));
  DEBUG_PRINT(GetCo2Raw());
  DEBUG_PRINT(F("\t"));
  DEBUG_PRINT(GetCo2Concentration());
  DEBUG_PRINT(F("\t Depth: "));
  DEBUG_PRINT(GetDepth());
  DEBUG_PRINT(F("\t Temp: "));
  DEBUG_PRINT(GetTemp());
  DEBUG_PRINT(F("\t Lum: "));
  DEBUG_PRINTLN(GetLumValue());
}
/* Bundle data into a package
 */
void BuildPackage() {
  InitializePackage();
  AppendLong(GetCH4ConcentrationEstimate());  // Package 1 (4 bytes)
  AppendLong(GetCH4Concentration());          // Package 2 (4 bytes)
  AppendLong(GetCo2Raw());                    // Package 3 (4 bytes)
  AppendLong(GetCo2Concentration());          // Package 4 (4 bytes)
  AppendLong(GetDepth());                     // Package 5 (4 bytes)
  AppendLong(GetTemp());                      // Package 6 (4 bytes)
  AppendInt(GetLumValue());                   // Package 7 (2 bytes)

  PrintData();
}

// Read last configured sampleInterval from EEPROM
void ReadSampleInterval() {
  unsigned int sampleInterval = EEPROM_READ_UINT(MEMADDR_SAMPLE_INTERVAL);
  sampleInterval              = max(LOGGING_SAMPLE_INTERVAL_MIN, min(LOGGING_SAMPLE_INTERVAL_MAX, sampleInterval));

  DEBUG_PRINT("Sample Interval: ");
  DEBUG_PRINTLN(sampleInterval);

  SetSampleInterval(sampleInterval);
}

// Set sampleInterval and save to EEPROM
void SetSampleInterval(unsigned int sampleInterval) {
  loggingSampleInterval = sampleInterval;
  DEBUG_PRINT("Writing Sample Interval to EEPROM: ");
  DEBUG_PRINTLN(sampleInterval);

  EEPROM_WRITE_INT(MEMADDR_SAMPLE_INTERVAL, sampleInterval);
}