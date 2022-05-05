#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

// Config
#include "../../config/constants.h"
#include "../../config/pinout.h"

// Function prototypes
#include "./prototypes.h"

// Initial modules
#include "../strategies/_strategies.h"
#include "./pinconfigs.h"

// Strategies
#include "../strategies/0_SystemTest.h"
#include "../strategies/1_Autonomous.h"
#include "../strategies/2_Service.h"

// System Control
#include "../system/blackbox.h"
#include "../system/control.h"
#include "../system/heartbeat.h"
#include "../system/power.h"

// Communication
#include "../communication/canister.h"
#include "../communication/lora.h"
#include "../communication/usb.h"

// Debug
#include "../debug/debug.h"

// Sensors
#include "../sensors/encoder.h"
#include "../sensors/logging.h"
#include "../sensors/motor.h"
#include "../sensors/rtc.h"
#include "../sensors/sdReader.h"

// Helper functions
#include "../tools/generalFunctions.h"
