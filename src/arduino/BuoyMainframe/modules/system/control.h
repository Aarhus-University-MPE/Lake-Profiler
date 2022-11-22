/*
  GeoRover System control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once
#include "../setup/modules.h"

// Enable individual module
void ModuleEnable(uint8_t module) {
  if (GetStatus(module))
    return;

  bool status = true;

  DEBUG_PRINT(F("Module Enable - "));
  DEBUG_PRINT(ModuleToString(module));

  switch (module) {
    case MODULE_PWR_PRIMARY:
      status = BatteryStatus(true);
      break;
    case MODULE_PWR_MOTOR:
      // No Primary Power, encoders enabled constantly.
      break;
    case MODULE_PWR_CANISTER:
      digitalWrite(PO_POWER_CANISTER, true);
      break;
    case MODULE_MOTOR:
      status = InitializeMotors();
      break;
    case MODULE_SD:
      status = InitializeSDReader();
      break;
    case MODULE_CLOCK:
      status = InitializeRTC();
      break;
    case MODULE_NA2:
      break;
    case MODULE_NA3:
      break;
    case MODULE_NA4:
      break;
    case MODULE_LORA:
      status = InitializeLora();
      break;
    case MODULE_COMM_LORA:
      status = LoRaJoin();
      break;
    case MODULE_COMM_CANISTER:
      status = InitializeCanister();
      break;
    case MODULE_COMM_DBG:
      status = InitializeDebugComm();
      break;
    case MODULE_BLACKBOX:
      status = InitBlackBox();
      break;
    case MODULE_CANISTER_HRTBEAT:
      break;
    case MODULE_DEBUG:
      status = DebugInitialize();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      break;
  }

  // Print initialization status
  if (module != MODULE_COMM_DBG) {
    if (!status) {
      DEBUG_PRINTLN(F(": Error"));
    } else {
      DEBUG_PRINTLN(F(": Enabled"));
    }
  }

  SetStatus(module, status);
  delay(10);
}

// Enable systems for specified mode
void ModuleEnableMode() {
  ModuleDisable();  // Disable secondary systems
  switch (mode) {
    case MODE_SYSTEMTEST:
      break;
    case MODE_AUTONOMOUS:
      ModuleEnable(MODULE_CLOCK);
      ModuleEnable(MODULE_MOTOR);
      break;
    case MODE_SERVICE:
      ModuleEnable(MODULE_PWR_MOTOR);
      ModuleEnable(MODULE_MOTOR);
      // ModuleEnable(MODULE_CLOCK);
      break;
    default:
      break;
  }
}

// Disables individual module
void ModuleDisable(int module) {
  if (!GetStatus(module))
    return;

  bool status = false;

  DEBUG_PRINT(F("Module Disable - "));
  DEBUG_PRINT(ModuleToString(module));

  switch (module) {
    case MODULE_PWR_PRIMARY:
      status = BatteryStatus();
      break;
    case MODULE_PWR_MOTOR:
      // No Primary Power, encoders enabled constantly.
      break;
    case MODULE_PWR_CANISTER:
      digitalWrite(PO_POWER_CANISTER, false);
      break;
    case MODULE_MOTOR:
      TerminateMotors();
      break;
    case MODULE_SD:
      TerminateSDReader();
      break;
    case MODULE_CLOCK:
      TerminateRTC();
      break;
    case MODULE_NA2:
      break;
    case MODULE_NA3:
      break;
    case MODULE_NA4:
      break;
    case MODULE_COMM_LORA:
      TerminateLora();
      break;
    case MODULE_COMM_CANISTER:
      TerminateCanister();
      break;
    case MODULE_BLACKBOX:
      break;
    case MODULE_CANISTER_HRTBEAT:
      break;
    case MODULE_DEBUG:
      DebugTerminate();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F(": Unable to disable"));
  } else {
    DEBUG_PRINTLN(F(": Disabled"));
  }

  SetStatus(module, status);
}

// Disables secondary systems
void ModuleDisable() {
  ModuleDisable(MODULE_PWR_MOTOR);
  ModuleDisable(MODULE_PWR_CANISTER);
  ModuleDisable(MODULE_MOTOR);
  // ModuleDisable(MODULE_CLOCK);
  // ModuleDisable(MODULE_COMM_LORA);
  // ModuleDisable(MODULE_COMM_CANISTER);
}

// Enables Primary Systems
void SystemEnablePrimary() {
  ModuleEnable(MODULE_SD);
  ModuleEnable(MODULE_BLACKBOX);
  if (LORA_ACTIVE) ModuleEnable(MODULE_LORA);
  if (LORA_ACTIVE) ModuleEnable(MODULE_COMM_LORA);
  ModuleEnable(MODULE_CLOCK);
}

// Runs system check and compares active modules to required
bool SystemCheck(int mode) {
  static bool status = false;
  SystemTest(false);

  switch (mode) {
    case MODE_AUTONOMOUS:
      status =
          ((((ToLong(SystemStatus) ^ SYSREQ_SAMPLE) & SYSREQ_SAMPLE) |
            (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT(F("ERROR Code: "));
        DEBUG_PRINTLN(((ToLong(SystemStatus) ^ SYSREQ_SAMPLE) &
                       SYSREQ_SAMPLE) |
                      (1L << MODULE_RESERVED));
      }
      break;
    default:
      break;
  }

  return status;
}

// Checks all Systems
void SystemCheck() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Running full system check"));
  DEBUG_PRINTLINE();

  BatteryStatus(true);
  SystemTest(false);

  SetStatus(MODULE_RESERVED, true);

  DEBUG_PRINT(F("System Status: "));
  DEBUG_PRINTLN(String(ToLong(SystemStatus)));
  DEBUG_PRINTLINE();
}

// Run full system check
void SystemTest(bool printRes) {
  SetStatus(MODULE_PWR_PRIMARY, BatteryStatus());
  // SetStatus(MODULE_PWR_MOTOR, GetStatus(MODULE_PWR_MOTOR));
  // SetStatus(MODULE_PWR_CANISTER, GetStatus(MODULE_PWR_CANISTER));
  // SetStatus(MODULE_MOTOR, MotorStatus());
  // SetStatus(MODULE_SD, SDReaderStatus());
  // SetStatus(MODULE_CLOCK, RTCStatus());
  // SetStatus(MODULE_COMM_LORA, LoraStatus());
  // SetStatus(MODULE_COMM_CANISTER, CanisterCommStatus());
  // SetStatus(MODULE_COMM_DBG, DebugCommStatus());
  // SetStatus(MODULE_BLACKBOX, BlackBoxStatus());
  // SetStatus(MODULE_RESERVED, true);
}