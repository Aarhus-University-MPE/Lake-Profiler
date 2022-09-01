#pragma once

// Modules
bool BuoyCommInitialize();
bool CH4Initialize();
bool CO2Initialize();
bool LumInitialize();
bool DepthInitialize();
bool TempInitialize();
bool DebugCommInitialize();

void BuoyCommTerminate();
void CH4Terminate();
void CO2Terminate();
void LumTerminate();
void DepthTerminate();
void TempTerminate();
void DebugCommTerminate();

bool BuoyCommTest();
bool CH4Test();
bool CO2Test();
bool LumTest();
bool DepthTest();
bool TempTest();

bool BuoyCommStatus();
bool CH4Status();
bool CO2Status();
bool LumStatus();
bool DepthStatus();
bool TempStatus();
bool DebugCommStatus();

void SensorRead();
void CH4Read();
void CO2Read();
void DepthRead();
void LumRead();
void TempRead();

bool CH4SendPackage();
bool CH4SendPackage();
bool CO2SendPackage();
bool DepthSendPackage();
bool LumSendPackage();
bool TempSendPackage();

long GetCH4Concentration();
long GetCH4ConcentrationEstimate();
long GetCo2Raw();
long GetCo2Concentration();
long GetDepth();
long GetTemp();
int GetLumValue();

void recvWithStartEndMarkersCH4();
void recvWithStartEndMarkersCO2();

void parseDataCH4(uint8_t size);
void parseDataCO2(uint8_t size);

// Communication
void BuoySendSensorError(byte sensorModule);
bool BuoySendPackage(uint8_t package[], uint8_t size);
bool BuoySendPackage(char package[], uint8_t size);
void BuoySendCommunicationError();
void BuoySendLogStart();
void parseCommand();
void parseCommandDebug();

// Package
void AppendFloat(float data, int index);
void AppendInt(int data, int index);
void AppendUnsignedInt(unsigned int data, int index);
void BuildPackage();
void ReadSampleInterval();
void SetSampleInterval(unsigned int sampleInterval);
void SetSampleID();
void GetSampleID();
void PrintPackage(uint8_t package[], uint8_t size);
void PrintPackage(char package[], uint8_t size);
void BundleIdentifier(PackageIdentifier identifier);

// System
bool SystemEnablePrimary();
bool SystemCheckModule(byte module);
bool SystemTestModule(byte module);

void LoggingStop();
