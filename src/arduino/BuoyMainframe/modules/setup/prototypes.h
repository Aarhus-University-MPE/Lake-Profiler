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
void initializeDebugComm();
void CountDownPrint();
const byte numChars = 32;

// Canister Comm
void recvWithStartEndMarkersCanister();
void CanisterLogStart();
void CanisterLogRead();
bool InitializeCanister();
bool CanisterCommStatus();
void parseCommandCan();

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
bool MotorStall();
bool GetMotorState();
bool MotorStatus();

// Encoder
void EncoderAInterrupt();
void EncoderBInterrupt();
void EncoderZInterrupt();

// Power
bool VoltageCheck();
bool BatteryStatus();
bool BatteryStatus(bool print);

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

// SD Reader
bool SDReaderStatus();
void SDQuery();
void SDSize(char fileName[]);
void SDDownload(char fileName[]);
void SDDelete(char fileName[]);
void SDCreate(char fileName[]);
void printFiles(File dir);
void appendCharArray(char *s, char c);
void appendCsv(char *s);

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
bool InitBlackBox();
bool BatteryStatus();
void TerminateMotors();
void TerminateSDReader();
void TerminateRTC();
void TerminateLora();
void TerminateCanister();

byte GetMode();

// additional
void parseCommand();
void parseCommandFiles();
void parseCommandStrategy();
void parseCommandModule();
void ModuleEnable();
void ModuleDisable();
void SystemTest(bool);
void SystemTest();
void SetStatus(bool);
void SetStatus(int, bool);

bool RTCStatus();
bool LoraStatus();
void SetAlarm();
void printTime(uint8_t);
void RTCPrint();

int BatteryLevel();

void MotorProcess();
void BlackBoxPrint();

bool SDWriteStream(char fileNameOrData[]);
bool SDWriteStreamNewLine();
bool SDOpenWriteStream(char fileName[]);
void SDQuit();

void HeartbeatBlackBox();

bool DebugInitialize();
void DebugTerminate();

void SystemEnablePrimary();

bool InputButtonDebounce();
bool ModeButtonDebounce();

void DataLogStop();
void DataLogStart();
void SetModeManual(byte modeSet);
void LoggingProcess();

void DataLogDeactivate();
void DataLogActivate();
