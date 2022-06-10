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

#pragma once

#include "../modules/setup/modules.h"

// User Inputs
#define PI_SWITCH_MODE       3
#define PI_BUTTON_MOTOR_DOWN 4
#define PI_BUTTON_MOTOR_UP   5

#define PT_SWITCH_MODE       digitalPinToInterrupt(PI_SWITCH_MODE)

// Power control (Relays)
#define PO_POWER_CANISTER    6
#define PO_POWER_ENCODER     7

// Sensors
#define PA_SENSOR_BATT       A0

// Encoder
#define PI_ENCODER_B         2
#define PI_ENCODER_Z         18
#define PI_ENCODER_A         19

#define PT_ENCODER_A         digitalPinToInterrupt(PI_ENCODER_A)
#define PT_ENCODER_B         digitalPinToInterrupt(PI_ENCODER_B)
#define PT_ENCODER_Z         digitalPinToInterrupt(PI_ENCODER_Z)

// Motor
#define PO_MOTOR_UP          8
#define PO_MOTOR_DOWN        10

// Communication
#define PO_SPISS_SDCARD      53  // 50 MISO, 51 MOSI, 52 SCK
// RTC Connection                 I2C SCL, SDA

#define COM_SERIAL_DBG       Serial
#define COM_SERIAL_CANISTER  Serial2
#define COM_SERIAL_LORA      Serial3
