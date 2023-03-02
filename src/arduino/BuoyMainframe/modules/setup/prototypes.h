/*
  Lake Profiler Function Templates protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#pragma once

#include "./modules.h"

void ModeSwitchInterrupt();

// System
void (*systemReset)(void) = 0;
void InitAllPins();
void InitButtons();
void InitMode();
void InitStrategyMethods();
unsigned long ToLong(bool b[]);

// USB Comm
bool DebugCommStatus();
void recvWithStartEndMarkers();
bool InitializeDebugComm();
void parseCommand();
void parseCommandFiles();
void parseCommandStrategy();
void parseCommandModule();
void parseCommandEncoder();
void parseCommandTimeEncoder();
void parseCommandPower();
void parseCommandLogging();
void parseCommandSample();
void CountDownPrint();
const byte numChars = 200;

// Canister Comm
void recvWithStartEndMarkersCanister();
void CanisterLogStart();
void CanisterLogRead();
bool InitializeCanister();
bool CanisterCommStatus();
void parseCommandCan(uint8_t size);

// Strategies
void StartStrategySystemTest();
void RunStrategySystemTest();
void FinishStrategySystemTest();
void SelectFunctionSystemTest();

void StartStrategyAutonomous();
void RunStrategyAutonomous();
void FinishStrategyAutonomous();
void SelectFunctionAutonomous();

void StartStrategyService();
void RunStrategyService();
void FinishStrategyService();
void SelectFunctionService();

// Mode switchers
boolean SetMode();
byte GetMode();
void ModeUpdater();

typedef void (*functionPtr)();

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
// [3][MODES_MAX] - function
functionPtr strategyMethods[4][MODES_MAX];

// Motors
void MotorMove(byte dir);
byte GetMotorState();
bool MotorStatus();
bool MotorPositionReached(uint8_t dir);
bool MotorPositionReached();
bool MotorPositionReachedEncoder(uint8_t dir);
bool MotorPositionReachedTimeEncoder(uint8_t dir);
void MotorSetPos();
int GetEncoderMode();
float GetTimeEncoderMotorSpeed();
void SetTimeEncoderMotorSpeed(float speed);

// Encoder
void EncoderAInterrupt();
void EncoderBInterrupt();
void EncoderZInterrupt();

void EncoderActivate();

int GetEncoderRotations();
int GetEncoderCount();

int GetEncoderRotationsTop();
int GetEncoderRotationsService();
int GetEncoderRotationsBottom();

int GetEncoderCountTop();
int GetEncoderCountService();
int GetEncoderCountBottom();

void SetEncoderRotationsTop(int value);
void SetEncoderRotationsBottom(int value);

void SetEncoderCountTop(int value);
void SetEncoderCountBottom(int value);

void SetEncoderTop();
void SetEncoderBottom();
void SetEncoderDepth(float depth);
void SetEncoderServiceDepth(float depth);
void EEPROMSetMotorPos();

void EncoderPrintPos();
void EncoderPrintPos(uint8_t direction);
bool EncoderWithinLimits();
void EEPROMGetMotorPos();

int GetEncoderPosition();
int GetEncoderPositionTop();
int GetEncoderPositionBottom();
int GetEncoderPositionService();

// Time Encoder
unsigned long GetTimeEncoderPosition();
unsigned long GetTimeEncoderTop();
unsigned long GetTimeEncoderBottom();
unsigned long GetTimeEncoderService();

int GetTimeMotorDirection();

void TimeEncoderActivate();
void TimeEncoderStart(uint8_t dir);
void TimeEncoderUpdate();

void UpdateMotorCompensation(uint8_t dir);
void SetTimeEncoderCompensationScale(float scale);
float GetTimeEncoderCompensationScale();

unsigned long GetTargetTimePosition(uint8_t dir);

void SetTimeEncoderTop();
void SetTimeEncoderBottom(unsigned long value);
void SetTimeEncoderService(unsigned long value);
void SetTimeEncoderBottom();

void SetTimeEncoderDepth(float depth);
void SetTimeEncoderServiceDepth(float depth);

void EEPROMSetTimeMotorPos();
void EEPROMGetTimeMotorPos();

void TimeEncoderPrintPos();
void TimeEncoderPrintPos(uint8_t direction);

void SetDepthSensorTopPosition(int topDepth);
long GetDepthSensorTopPosition();

void TimeEncoderPrintTarget(uint8_t dir);
unsigned long GetTimeEncoderTarget(uint8_t dir);

// Power
bool VoltageCheck();
bool BatteryStatus();
bool BatteryStatus(bool print);
void SetBatteryMinLevel(uint8_t batteryLevel);
uint8_t GetBatteryMinLevel();
int BatteryLevel();
uint8_t BatteryLevelHex();
uint8_t BatteryVoltageHex();

// System Status
bool SystemStatus[MODULE_COUNT];

bool GetStatus(int module) {
  return SystemStatus[module];
}

void SetStatus(int module, bool status) {
  SystemStatus[module] = status;
}

void SetStatus(bool status) {
  for (int i = 0; i < MODULE_COUNT; i++) {
    SystemStatus[i] = status;
  }

  SystemStatus[MODULE_RESERVED] = true;
}

bool SystemCheck(int mode);
void SystemCheck();

void ModuleEnableMode(int mode);
void ModuleEnableMode();

void ModuleEnable();
void ModuleEnable(uint8_t module);

// SD Reader
bool SDReaderStatus();
void SDQuery();
void SDSize(char fileName[]);
void SDDownload(char fileName[]);
void SDDelete(char fileName[]);
bool SDCreate(char fileName[]);
bool SDCreate(char fileName[], bool customFileEnd);
void SDWipe();
void printFiles(File dir);
void appendCharArray(char *s, char c);
void appendCsv(char *s);
void printDirectory(File dir, int numTabs);

// Blackbox
bool BlackBoxStatus();

void BlackBoxAppendln();
void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppendln(String blackBoxInput);

void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppendln(int blackBoxInput);

void BlackBoxAppend(long int blackBoxInput, int Type);
void BlackBoxAppendln(long int blackBoxInput, int Type);

void BlackBoxAppend(float blackBoxInput);
void BlackBoxAppendln(float blackBoxInput);

void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput);

void BlackBoxAppend(unsigned long blackBoxInput);
void BlackBoxAppendln(unsigned long blackBoxInput);

// System Control
bool InitializeMotors();
bool InitializeSDReader();
bool InitializeRTC();
bool InitializeLora();
bool InitializeCanister();
bool LoRaInitializeBroadcastLog();
bool LoRaInitializeBroadcastData();
bool InitBlackBox();
bool BatteryStatus();
void TerminateMotors();
void TerminateSDReader();
void TerminateRTC();
void TerminateLora();
void TerminateCanister();

// Autonomy
void AutonomyProcess();
void ButtonOverride();
bool AutonomyAwaitHandshake();
bool AutonomyAwaitAcknowledge();
bool AutonomyStartLog();
void AutonomyStopLog();
void AutonomyState();
bool AutonomyPowerCheck();
void AutonomyStartCanister();
void AutonomyErrorStop();
void AutonomyReset();
bool LoggingActive();

bool DataReceived();
bool AcknowledgeReceived();

// Data Recoder
bool InitializeLoggingFiles();
void AppendToLog(char *logInput, bool endLine);
void AppendToLog(String logInput, bool endLine);
void AppendToLog(String logInput);
void AppendToLog(char *logInput);
void AppendToData(char *dataInput, bool endLine);
void AppendToData(char *dataInput);
void AppendToData(uint8_t *dataInput, uint8_t size, bool endLine);
void AppendToData(uint8_t *dataInput, uint8_t size);
void AppendPackageIdentifierToData();
unsigned long GetDataLines();
void AppendIndexToData();
void AppendData(uint8_t size);
void AppendData();
bool LogFileLoad();
bool DataFileLoad();
void TimeStampData();
void TimeStampLog();
bool DataReadLine(uint8_t *package, int &size);
bool LogReadLine(uint8_t *package, int &size);

// additional
void ModuleDisable();
void ModuleDisable(int module);
void SystemTest(bool);
void SystemTest();
void SetStatus(bool);
void SetStatus(int, bool);

bool RTCStatus();
bool LoraStatus();
void SetAlarm();
void SetAlarm(bool skipHour);
int GetAlarmFrequency();

void printTime(uint8_t);
void RTCPrint();

void MotorProcess();
void BlackBoxPrint();

// SD Write Steam (Continuous writing to same focument)
bool SDWriteStream(char fileNameOrData[], bool customFileEnd);
bool SDWriteStream(char fileNameOrData[]);
bool SDWriteStream(uint8_t data[], uint8_t size, bool customFileEnd);
bool SDWriteStream(uint8_t data[], uint8_t size);
bool SDWriteStreamNewLine();
bool SDOpenWriteStream(char fileName[], bool customEndLine);
void SDQuit();

void HeartbeatBlackBox();

bool DebugInitialize();
void DebugTerminate();

void SystemEnablePrimary();

bool UpButtonDebounce();
bool DownButtonDebounce();
bool ModeButtonDebounce();

bool DataLogStart();
void DataLogStop();
void DataLogInitialized();
void SetModeManual(uint8_t modeSet);
void LoggingProcess();
int GetWarmupTime();
void SetWarmupTime(int warmupTime);

void DataLogActivate();
bool DataLogActive();

float BatteryVoltage();
void parsePackage(uint8_t size);
void PrintPackageInfo(uint8_t size);
void parseLogStart();
void GetTimeStamp(char *fileName);

// RTC
void SetClock();
void UpdateUnixTime();
void EnableAlarm(uint8_t alarm);
void PrintAlarmTime();

// Clock
void parseCommandClock();
void SetClockSec(uint8_t hexValue);
void SetClockMin(uint8_t hexValue);
void SetClockHour(uint8_t hexValue);
void SetClockDay(uint8_t hexValue);
void SetClockDate(uint8_t hexValue);
void SetClockMonth(uint8_t hexValue);
void SetClockYear(uint8_t hexValue);

// Alarms
void parseCommandAlarm();
void SetAlarmSec(uint8_t hexValue);
void SetAlarmMin(uint8_t hexValue);
void SetAlarmHour(uint8_t hexValue);
void SetAlarmDay(uint8_t hexValue);
void SetAlarmDate(uint8_t hexValue);
void SetAlarmMonth(uint8_t hexValue);
void SetAlarmStartHour(uint8_t startHour);
void SetAlarmFrequency(uint8_t frequency);
bool AlarmStatus(uint8_t src);

void InitializeAlarm();
void UpdateAlarm();
void UpdateAlarmTimings();
void SetAlarm();
void SetAlarm(uint8_t hour);
uint8_t NextAlarm();
uint8_t SetAlarmHourFromNow();

// LoRa
static int at_send_check_response(bool printResponse, String p_ack_str, unsigned long timeout_ms, String p_cmd_str, ...);
static void recv_prase(char *p_msg);

bool LoRaJoin();
bool LoRaConfigure();
bool LoRaBroadcastLog();
bool LoRaBroadcastData();
void LoRaBroadcastBegin();
void LoRaBroadcastLowPower();
void LoRaBroadcastLogBegin();
void LoRaBroadcastPowerLevel();
void LoRaBroadcastPosError();
void LoRaHeartbeat();

unsigned int IncrementSampleID();
unsigned int GetSampleID();
void SetSampleID(unsigned int idValue);
void IdStampData();

long GetLatestDepth();