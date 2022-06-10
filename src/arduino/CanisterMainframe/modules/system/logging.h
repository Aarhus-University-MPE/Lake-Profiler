#pragma once
#include "../setup/modules.h"

unsigned int sampleID, loggingSampleInterval;
bool systemActive              = false;
unsigned int sampleIndex       = 0;
unsigned long lastMillisSensor = 0;
uint8_t packageSize            = 23;
uint8_t package[25];

bool LoggingStart() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Logging Process"));
  DEBUG_PRINTLINE();

  sampleIndex      = 0;
  lastMillisSensor = 0;

  SetSampleID();
  ReadSampleInterval();

  SystemEnablePrimary();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Logging Started"));
  DEBUG_PRINTLINE();

  // BuoySendLogStart();
  systemActive = true;

  return systemActive;
}

void LoggingStop() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Stopping Data Logging"));
  DEBUG_PRINTLINE();
  systemActive = false;
}

// Package header
bool SendHeader() {
  BundleIdentifier(PACKAGE_0);
  return BuoySendPackage(package, 5);
}

// Package footer
bool SendFooter() {
  BundleIdentifier(PACKAGE_END);
  return BuoySendPackage(package, 5);
}
// Send all packages
bool SendPackage() {
  DEBUG_PRINTLN(F("Sending Package: "));

  // if (!SendHeader()) return false;
  DEBUG_PRINTLINE();
  // if (!CH4SendPackage()) return false;
  // if (!CO2SendPackage()) return false;
  // if (!DepthSendPackage()) return false;
  // if (!TempSendPackage()) return false;
  // if (!LumSendPackage()) return false;

  BuildPackage();
  if (!BuoySendPackage(package, packageSize)) return false;

  // if (!SendFooter()) return false;
  DEBUG_PRINTLINE();
  return true;
}

// Sensor Package Broadcast
void SensorBroadcast() {
  if (millis() - lastMillisSensor < loggingSampleInterval) {
    return;
  }

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

void SetSampleID() {
  sampleID = EEPROM_READ_UINT(MEMADDR_SAMPLEID) + 1;
  EEPROM_WRITE_INT(MEMADDR_SAMPLEID, sampleID);
}

unsigned int SampleIndex() {
  sampleIndex++;
  return sampleIndex;
}

/* Add identifier to package
  Bundle ID
  0-9: Status
*/
void BundleIdentifier(PackageIdentifier identifier) {
  // Message Wrapper

  // Bundle ID
  package[0] = identifier;

  // Sample Routine ID
  AppendUnsignedInt(sampleID, 1);

  // Sample index
  sampleIndex = SampleIndex();
  AppendUnsignedInt(sampleIndex, 3);
}

void AppendFloat(float data, int index) {
  union unpack pack;
  pack.f = data;

  for (int i = 0; i < 4; i++) {
    package[index + i] = pack.b[i];
  }
}

void AppendInt(int data, int index) {
  union unpack pack;
  pack.i = data;

  package[index]     = pack.b[0];
  package[index + 1] = pack.b[1];

  // for (int i = 0; i < 2; i++) {
  //   package[index + i] = pack.b[i];
  // }
}
void AppendUnsignedInt(unsigned int data, int index) {
  union unpack pack;
  pack.ui = data;

  package[index]     = pack.b[0];
  package[index + 1] = pack.b[1];

  // for (int i = 0; i < 2; i++) {
  //   package[index + i] = pack.b[i];
  // }
}
/* Bundle data into a package
 */
void BuildPackage() {
  BundleIdentifier(PACKAGE_0);  // Byte 0-5
  // package[0] = '$';
  AppendFloat(GetCH4Concentration(), 6);   // Byte 6-9
  AppendFloat(GetCo2Concentration(), 10);  // Byte 10-13
  AppendFloat(GetDepth(), 14);             // Byte 14-17
  AppendFloat(GetTemp(), 18);              // Byte 18-21
  AppendInt(GetLumValue(), 22);            // Byte 22-23
  // TimeStamp();                            // Timestamp added at top
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