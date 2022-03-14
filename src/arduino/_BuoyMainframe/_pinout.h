/*
  Lake Profiler system pinout

  Naming pattern (all uppercase):
  P   + <I:input, T: interrupt input, A:analog input, O:output, P:pwm output>_
    + <general part name>_
    + <purpose>_
    + <where applicable: left, right, top, bottom, position etc...>

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// External Inputs
#define PI_BUTTON_MODE_AUTONOMOUS   19
#define PI_BUTTON_MODE_SERVICE      18 

#define PT_BUTTON_MODE_AUTONOMOUS   digitalPinToInterrupt(PI_BUTTON_MODE_AUTONOMOUS)
#define PT_BUTTON_MODE_SERVICE      digitalPinToInterrupt(PI_BUTTON_MODE_SERVICE)

// Power control (Relays)
#define PO_POWER_SECONDARY    38
#define PO_POWER_MOTOR        42
#define PO_POWER_CANISTER     44

// Motor
#define PO_MOTOR_P1           10
#define PO_MOTOR_P2           11

// Communication
#define PO_SPISS_SDCARD       28
#define PO_SPISS_LORA         12

#define COM_SERIAL_DBG        Serial
#define COM_SERIAL_CANISTER   Serial2

// Heartbeat
#define PI_CANISTER_HRTBEAT  2
#define PO_CANISTER_HRTBEAT  3
#define PI_INT_HRTBEAT      digitalPinToInterrupt(PI_CANISTER_HRTBEAT)

// Reset pin
#define PI_CANISTER_RST       RESET
#define PO_CANISTER_RST       4