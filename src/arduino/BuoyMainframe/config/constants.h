/*
  Lake Profiler system constants

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#pragma once

#include "../modules/setup/modules.h"

// ------------------------------------------------------------ //
//                            DEBUG                             //
// ------------------------------------------------------------ //

// Debug configuration flag - Comment out to unset
#define DEBUG_BLACKBOX_AND_PRINT
// #define DEBUG_BLACKBOX
// #define DEBUG_SERIAL
// #define DEBUG_NONE

// Executes if DEBUG flag is set.
#if defined(DEBUG_BLACKBOX_AND_PRINT)
#define DBG_ONLY(x) x
#define DEBUG_PRINT(x) \
  BlackBoxAppend(x);   \
  Serial.print(x)
#define DEBUG_PRINT2(x, y) \
  BlackBoxAppend(x, y);    \
  Serial.print(x, y)
#define DEBUG_PRINTLN(x) \
  BlackBoxAppendln(x);   \
  Serial.println(x)
#define DEBUG_PRINTLN2(x, y) \
  BlackBoxAppendln(x, y);    \
  Serial.println(x, y)
#define DEBUG_PRINTLINE() \
  Serial.println(F("------------------------------------"))
#define DEBUG_WRITE(x) Serial.write(x)
#define RECEIVE_CMDS() recvWithStartEndMarkers()
#endif

#if defined(DEBUG_BLACKBOX)
#define DBG_ONLY(x)
#define DEBUG_PRINT(x)       BlackBoxAppend(x)
#define DEBUG_PRINT2(x, y)   BlackBoxAppend(x, y)
#define DEBUG_PRINTLN(x)     BlackBoxAppendln(x)
#define DEBUG_PRINTLN2(x, y) BlackBoxAppendln(x, y)
#define DEBUG_PRINTLINE()
#define DEBUG_WRITE(x)
#define RECEIVE_CMDS()
#endif

#if defined(DEBUG_SERIAL)
#define DBG_ONLY(x)          x
#define DEBUG_PRINT(x)       Serial.print(x)
#define DEBUG_PRINT2(x, y)   Serial.print(x, y)
#define DEBUG_PRINTLN(x)     Serial.println(x)
#define DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define DEBUG_PRINTLINE()    Serial.println(F("------------------------------------"))
#define DEBUG_WRITE(x)       Serial.write(x)
#define RECEIVE_CMDS()       recvWithStartEndMarkers()
#endif

#if defined(DEBUG_NONE)
#define DBG_ONLY(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINT2(x, y)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLN2(x, y)
#define DEBUG_PRINTLINE()
#define DEBUG_WRITE(x)
#define RECEIVE_CMDS()
#endif

#define ToBoolString(m)          ((m) == 1 ? "T" : ((m) == 0 ? "F" : "Unknown"))
// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //
#define SystemVersion            "0.3.54"

// Binary codes for Status LED flags, Red Yellow Green
#define BINARY_CODE_LED_GRN      B001  // 001
#define BINARY_CODE_LED_YEL      B010  // 010
#define BINARY_CODE_LED_RED      B100  // 100

// LED blink durations (ms)
#define LED_BLINK_VERY_LONG      2000
#define LED_BLINK_LONG           1000
#define LED_BLINK_SHORT          250
#define LED_BLINK_VERY_SHORT     100

// Button debounce
#define BUTTON_DBOUNCE_TIME      50
#define BUTTON_DBOUNCE_TIME_LONG 1000

// Sensor and Module status
#define SYSTEM_CHECK_DT          1000
#define SYSTEM_REBOOT_DT         5000

#define MODULE_COUNT             17

#define MODULE_PWR_PRIMARY       0
#define MODULE_PWR_MOTOR         1
#define MODULE_PWR_CANISTER      2
#define MODULE_MOTOR             3
#define MODULE_SD                4
#define MODULE_CLOCK             5
#define MODULE_NA2               6
#define MODULE_NA3               7
#define MODULE_NA4               8
#define MODULE_NA5               9
#define MODULE_COMM_LORA         10
#define MODULE_COMM_CANISTER     11
#define MODULE_COMM_DBG          12
#define MODULE_BLACKBOX          13
#define MODULE_CANISTER_HRTBEAT  14
#define MODULE_DEBUG             15
#define MODULE_RESERVED          16

// Requirements to run a sample
const unsigned long SYSREQ_SAMPLE =
    (1L << MODULE_PWR_PRIMARY) +
    (1L << MODULE_PWR_MOTOR) +
    (1L << MODULE_PWR_CANISTER) +
    (1L << MODULE_MOTOR) +
    (1L << MODULE_SD) +
    (1L << MODULE_COMM_CANISTER);

#define ModuleToString(m) \
  ((m) == 0 ? "Primary Power" : ((m) == 1 ? "Motor Power" : ((m) == 2 ? "Canister Power" : ((m) == 3 ? "Motor" : ((m) == 4 ? "SD reader" : ((m) == 5 ? "Realtime Clock" : ((m) == 6 ? "NA2" : ((m) == 7 ? "NA3" : ((m) == 8 ? "NA4" : ((m) == 9 ? "NA5" : ((m) == 10 ? "Long Range Communication (LORA)" : ((m) == 11 ? "Canister Communication" : ((m) == 12 ? "Debug Communication" : ((m) == 13 ? "Blackbox" : ((m) == 14 ? "Canister Heartbeat" : ((m) == 15 ? "Debug" : ((m) == 16 ? "Reserved" : "Unknown")))))))))))))))))

// ------------------------------------------------------------ //
//                       STRATEGY MODES                         //
// ------------------------------------------------------------ //

#define MODES_MAX                    3  // Total number of modes
#define MODES_MIN_BROWSABLE          1  // Minimum mode index, that could be set via mode button.

#define MODE_SYSTEMTEST              0  // Test main systems
#define MODE_AUTONOMOUS              1  // Autonomous driving mode
#define MODE_SERVICE                 2  // Service mode

#define ModeToString(m)              ((m) == 0 ? "System Test" : ((m) == 1 ? "Autonomous" : ((m) == 2 ? "Service" : "Unknown")))

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT              1  // times per minute
#define HRTBEAT_DT_OUT               60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD             300000  // Treshold for late heartbeat

#define BACKUP_RST_DT                300000  // Time between each reset attempt

#define HRTBEAT_DT_LOG               300000  // Time between system status log

// ------------------------------------------------------------ //
//                           BATTERY                            //
// ------------------------------------------------------------ //

#define BATTERY_MIN_LEVEL            50

// ------------------------------------------------------------ //
//                           MOTORS                             //
// ------------------------------------------------------------ //

#define MOTOR_SPEED                  100
#define MOTOR_ACCEL                  20
#define MOTOR_STEPS                  5
#define MOTOR_POS_TOP                500
#define MOTOR_POS_BOT                -500
#define MOTOR_DIR_UP                 1
#define MOTOR_DIR_DOWN               2
#define MOTOR_DIR_HALT               0

// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME            500
#define ESTOP_DEBOUNCE_TIME          1000

// DEBUG
#define DEBUG_BAUDRATE               115200

// Canister Comm (RS232)
#define CANISTER_BAUDRATE            19200

// LORA
#define LORA_FREQUENCY               434.0
#define CLIENT_ADDRESS               1
#define SERVER_ADDRESS               2

// ------------------------------------------------------------ //
//                           EEPROM                             //
// ------------------------------------------------------------ //
#define EEPROM_READ_INT(addr)        (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr + 1) << 8) & 0xFF00))

#define EEPROM_WRITE_INT(index, int) EEPROM.put(index, int)

// Modes
#define MEMADDR_LASTMODE             0
#define MEMADDR_ENCODER_COUNT        2
#define MEMADDR_ENCODER_ROTATION     4

// motor calibration cache

// ------------------------------------------------------------ //
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK               '<'
#define CMD_END_MARK                 '>'

#define CMD_FILES                    'F'
#define CMD_FILES_LIST               'L'
#define CMD_FILES_SIZE               'S'
#define CMD_FILES_DOWNLOAD           'D'
#define CMD_FILES_CREATE             'C'
#define CMD_FILES_REMOVE             'R'
#define CMD_FILES_WRITE              'W'
#define CMD_FILES_WRITENEWLINE       'N'
#define CMD_FILES_QUIT               'Q'
#define CMD_FILES_BLCKBOX            'B'
#define CMD_FILES_BLCKBOXEMPTY       'E'

#define CMD_STRATEGY                 'S'
#define CMD_STRATEGY_SET             'S'
#define CMD_STRATEGY_FUNCTION        'F'
#define CMD_STRATEGY_OVERRIDE        'O'

#define CMD_BACKUP                   'B'
#define CMD_BACKUP_RST               'R'
#define CMD_BACKUP_PRIMSTATUS        'S'
#define CMD_BACKUP_HB                'H'

#define CMD_MODULE                   'M'
#define CMD_MODULE_ENABLE            'E'
#define CMD_MODULE_DISABLE           'D'
#define CMD_MODULE_OVERRIDE          'O'
#define CMD_MODULE_STATUS            'S'
#define CMD_MODULE_RESET             'R'
