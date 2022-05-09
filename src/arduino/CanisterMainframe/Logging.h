void CanisterSendPackage(uint8_t package[], uint8_t size);
void CH4Read();
void CO2Read();
void DepthRead();
void LumRead();
void TempRead();
float GetCH4Concentration();
float GetCo2Concentration();
float GetDepth();
float GetTemp();
int GetLumValue();
void BuildPackage();
void SensorRead();
bool SystemEnablePrimary();
void ReadSampleInterval();
void SetSampleInterval(unsigned int sampleInterval);
void SetSampleID();
void AppendFloat(float data, int index);
void AppendInt(int data, int index);
void AppendUnsignedInt(unsigned int data, int index);
void CanisterSendSensorError(byte sensorModule);

enum PackageIdentifier {
  PACKAGE_ERROR        = 0,
  PACKAGE_ERROR_SENSOR = 1,
  PACKAGE_START        = 2,
  PACKAGE_STATUS       = 3,
  PACKAGE_0            = 10,
  PACKAGE_1            = 11,
  PACKAGE_2            = 12,
  PACKAGE_3            = 13,
  PACKAGE_4            = 14,
};

union unpack {
  float f;
  int i;
  unsigned int ui;
  byte b[4];
};

unsigned int sampleID, loggingSampleInterval;
bool systemActive              = false;
unsigned int sampleIndex       = 0;
unsigned long lastMillisSensor = 0;
const byte packageSize         = 22;
uint8_t package[packageSize];

bool LoggingStart() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Logging Process"));
  DEBUG_PRINTLINE();

  sampleIndex      = 0;
  lastMillisSensor = 0;

  SetSampleID();
  ReadSampleInterval();

  if (!SystemEnablePrimary()) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Logging Start Error!"));
    systemActive = false;
  }
  DEBUG_PRINTLINE();
  systemActive = true;

  return systemActive;
}

void SensorBroadcast() {
  if (millis() - lastMillisSensor < loggingSampleInterval) {
    return;
  }

  lastMillisSensor = millis();

  // Rebuild Package with latest data
  BuildPackage();

  // Broadcast to Buoy
  CanisterSendPackage(package, packageSize);
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
  DepthRead();
  // LumRead(); // Analog sensor, no need to constantly update
  TempRead();
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
  // Bundle ID
  package[0] = identifier;

  // Sample Routine ID
  AppendUnsignedInt(sampleID, 1);

  // Sample index
  AppendUnsignedInt(SampleIndex(), 3);
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
  BundleIdentifier(PACKAGE_0);            // Byte 0,1,2,3
  AppendFloat(GetCH4Concentration(), 5);  // Byte 5,6,7,8
  AppendFloat(GetCo2Concentration(), 9);  // Byte 9,10,11,12
  AppendFloat(GetDepth(), 13);            // Byte 13,14,15,16
  AppendFloat(GetTemp(), 17);             // Byte 17,18,19,20
  AppendInt(GetLumValue(), 21);           // Byte 21,22
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