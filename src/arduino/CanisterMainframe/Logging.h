void CanisterSendPackage(uint8_t package[], uint8_t size);
void CH4Read();
void CO2Read();
void DepthRead();
void LumRead();
void TempRead();

const byte packageSize = 100;
uint8_t package[packageSize];

void LoggingStart() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Logging Process"));
  DEBUG_PRINTLINE();
}
unsigned long lastMillisSensor;

void SensorProcess() {
  // Read sensor data

  if (millis() - lastMillisSensor < SENSOR_LOG_PERIOD) {
    return;
  }

  lastMillisSensor = millis();

  CanisterSendPackage(package, packageSize);
}

// Process all incoming sensors data
void SensorRead() {
  CH4Read();
  CO2Read();
  DepthRead();
  LumRead();
  TempRead();
}

// Bundle data into a package
void BuildPackage() {  // TODO: Endian
}