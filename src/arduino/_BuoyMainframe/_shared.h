// Function Prototypes
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <EEPROM.h>
#include "_GeneralFunctions.h"


// System
void(* systemReset) (void) = 0;
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
byte mode;
byte modeCycle;
byte prevMode;
boolean isModeUpdated = false;
boolean SetMode(byte newMode);
void ModeUpdater();

typedef void (*functionPtr)();

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
// [3][MODES_MAX] - function
functionPtr strategyMethods[4][MODES_MAX];


// Motors
void MotorMove();
void MotorSet(byte dir);
bool MotorStall();
bool GetMotorState();
bool MotorStatus();


// Power
bool VoltageCheck();  
bool BatteryStatus();


// System Status
bool SystemStatus[MODULE_COUNT];
bool GetStatus(int module)
{
  return SystemStatus[module];
}
void SetStatus(int module, bool status)
{
  SystemStatus[module] = status;
}
void SetStatus(bool status)
{
  for (int i = 0; i < MODULE_COUNT; i++)
  {
    SystemStatus[i] = status;
  }
  SystemStatus[MODULE_RESERVED] = true;
}
bool SystemCheck(int mode);
void ModuleEnableMode(int mode);


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
void BlackBoxAppendln(String blackBoxInput);
void BlackBoxAppendln(byte blackBoxInput);
void BlackBoxAppendln(bool blackBoxInput);
void BlackBoxAppendln(char blackBoxInput);
void BlackBoxAppendln(int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput);
void BlackBoxAppendln(unsigned long blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput, int Type);

void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppend(byte blackBoxInput);
void BlackBoxAppend(bool blackBoxInput);
void BlackBoxAppend(char blackBoxInput);
void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppend(unsigned long blackBoxInput);
void BlackBoxAppend(long int blackBoxInput, int Type);