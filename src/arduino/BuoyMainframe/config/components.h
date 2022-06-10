#pragma once

#define MODULE_COUNT            17

#define MODULE_PWR_PRIMARY      0
#define MODULE_PWR_MOTOR        1
#define MODULE_PWR_CANISTER     2
#define MODULE_MOTOR            3
#define MODULE_SD               4
#define MODULE_CLOCK            5
#define MODULE_NA2              6
#define MODULE_NA3              7
#define MODULE_NA4              8
#define MODULE_LORA             9
#define MODULE_COMM_LORA        10
#define MODULE_COMM_CANISTER    11
#define MODULE_COMM_DBG         12
#define MODULE_BLACKBOX         13
#define MODULE_CANISTER_HRTBEAT 14
#define MODULE_DEBUG            15
#define MODULE_RESERVED         16

// Requirements to run a sample
const unsigned long SYSREQ_SAMPLE =
    (1L << MODULE_PWR_PRIMARY) +
    (1L << MODULE_PWR_MOTOR) +
    (1L << MODULE_PWR_CANISTER) +
    (1L << MODULE_MOTOR) +
    (1L << MODULE_SD) +
    (1L << MODULE_COMM_CANISTER);

#define ModuleToString(m) \
  ((m) == 0 ? "Primary Power" : ((m) == 1 ? "Motor Power" : ((m) == 2 ? "Canister Power" : ((m) == 3 ? "Motor" : ((m) == 4 ? "SD reader" : ((m) == 5 ? "Realtime Clock" : ((m) == 6 ? "NA2" : ((m) == 7 ? "NA3" : ((m) == 8 ? "NA4" : ((m) == 9 ? "LoRa Module" : ((m) == 10 ? "LoRa Network Connection" : ((m) == 11 ? "Canister Communication" : ((m) == 12 ? "Debug Communication" : ((m) == 13 ? "Blackbox" : ((m) == 14 ? "Canister Heartbeat" : ((m) == 15 ? "Debug" : ((m) == 16 ? "Reserved" : "Unknown")))))))))))))))))

#define CanisterModuleToString(m) \
  ((m) == 0 ? "Buoy Communication" : ((m) == 1 ? "Buoy Heartbeat" : ((m) == 2 ? "CH4 Sensor" : ((m) == 3 ? "CO2 Sensor" : ((m) == 4 ? "Cyclops-7F Sensor" : ((m) == 5 ? "Depth Sensor" : ((m) == 6 ? "Temperature Sensor" : ((m) == 7 ? "Debug Communication" : ((m) == 8 ? "Reserved" : ("Unknown"))))))))))
