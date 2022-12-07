/*
  Lake Profiler EEPROM memory allocation

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#pragma once

// Read/Write functions
#define EEPROM_READ_INT(addr)                  (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr + 1) << 8) & 0xFF00))
#define EEPROM_READ_UINT(addr)                 (unsigned int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr + 1) << 8) & 0xFF00))

#define EEPROM_READ_ULONG(addr, ulongValue)    EEPROM.get(addr, ulongValue)
//(unsigned long)((unsigned long)((EEPROM.read(addr) << 0) & 0xFF) + (unsigned long)((EEPROM.read(addr + 1) << 8) & 0xFF00) + (unsigned long)((EEPROM.read(addr + 2) << 16) & 0xFF0000) + (unsigned long)((EEPROM.read(addr + 3) << 24) & 0xFF000000))

#define EEPROM_WRITE_UINT(addr, uintValue)     EEPROM.put(addr, uintValue)
#define EEPROM_WRITE_INT(addr, intValue)       EEPROM.put(addr, intValue)

// Encoder
#define MEMADDR_ENCODER_COUNT                  0
#define MEMADDR_ENCODER_ROTATION               2

#define MEMADDR_ENCODER_ROTATION_TOP           4
#define MEMADDR_ENCODER_ROTATION_BOTTOM        6
#define MEMADDR_ENCODER_ROTATION_SERVICE       8

#define MEMADDR_ENCODER_COUNT_TOP              10
#define MEMADDR_ENCODER_COUNT_BOTTOM           12
#define MEMADDR_ENCODER_COUNT_SERVICE          14

// Alarm
#define MEMADDR_ALARM_FREQUENCY                16
#define MEMADDR_ALARM_START                    18

// Battery
#define MEMADDR_BATTERY_MIN                    20

#define MEMADDR_SYSTEM_WARMUP                  22

// Sample Information
#define MEMADDR_SAMPLEID                       24

// Time-based Encoder
#define MEMADDR_ENCODER_MODE                   26

#define MEMADDR_TIMEENCODER_BOTTOM             28
#define MEMADDR_TIMEENCODER_SERVICE            32
#define MEMADDR_TIMEENCODER_CURRENT            36
#define MEMADDR_TIMEENCODER_COMPENSATION_SCALE 40

#define MEMADDR_DEPTH_TOP_POSITION             44

#define MEMADDR_MOTOR_SPEED                    48
