/*
  Lake Profiler EEPROM memory allocation

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#pragma once

// Read/Write functions
#define EEPROM_READ_INT(addr)            (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr + 1) << 8) & 0xFF00))

#define EEPROM_WRITE_INT(addr, intValue) EEPROM.put(addr, intValue)

// Encoder
#define MEMADDR_ENCODER_COUNT            0
#define MEMADDR_ENCODER_ROTATION         2

#define MEMADDR_ENCODER_ROTATION_TOP     4
#define MEMADDR_ENCODER_ROTATION_BOTTOM  6

#define MEMADDR_ENCODER_COUNT_TOP        8
#define MEMADDR_ENCODER_COUNT_BOTTOM     10

// Alarm
#define MEMADDR_ALARM_FREQUENCY          12
#define MEMADDR_ALARM_START              14

// Battery
#define MEMADDR_BATTERY_MIN              16

#define MEMADDR_SYSTEM_WARMUP            18

// Sample Information
#define MEMADDR_SAMPLEID                 20