#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

// Config
#include "../../config/constants.h"
#include "../../config/pinout.h"

// Tools
#include "../tools/functions.h"

// Function prototypes
#include "./prototypes.h"

// Initial modules
#include "./pinconfigs.h"

// System Control
#include "../system/control.h"
#include "../system/logging.h"

// Communication
#include "../communication/buoy.h"
#include "../communication/usb.h"

// Sensors
#include "../sensors/CH4Contros.h"
#include "../sensors/CO2ProOceanus.h"
#include "../sensors/DepthBar100.h"
#include "../sensors/LumCyclops.h"
#include "../sensors/TempTSYS01.h"
