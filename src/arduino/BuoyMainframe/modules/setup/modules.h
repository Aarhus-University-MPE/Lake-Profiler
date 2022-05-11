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

// Helper functions
#include "../tools/generalFunctions.h"

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
#include "../system/logging.h"
#include "../system/power.h"

// Communication
#include "../communication/canister.h"
#include "../communication/lora.h"
#include "../communication/usb.h"

// Debug
#include "../debug/debug.h"

// Components
#include "../components/encoder.h"
#include "../components/motor.h"
#include "../components/rtc.h"
#include "../components/sdReader.h"
