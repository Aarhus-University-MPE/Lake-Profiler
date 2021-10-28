/*
  Lake Profiler system constants

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// #include <_shared.h>

// ------------------------------------------------------------ //
//                            DEBUG                             //
// ------------------------------------------------------------ //

// Debug configuration flag - Comment out to unset
#define DEBUG

// Executes one line of code only if DEBUG flag is set.
#if defined(DEBUG)
  #define DBG_ONLY(x) x
  #define DEBUG_PRINT(x) Serial.print(x); BlackBoxAppend(x);
  #define DEBUG_PRINTLN(x) Serial.println(x); BlackBoxAppendln(x);
  #define DEBUG_PRINTLN2(x,y) Serial.println(x,y); BlackBoxAppendln(x,y);
  #define DEBUG_WRITE(x) Serial.write(x)
  #define DEBUG_PRINTLINE() DEBUG_PRINTLN("------------------------------------"); BlackBoxAppendln("------------------------------------");
  #define RECEIVE_CMDS()  recvWithStartEndMarkers()
#else
  #define DBG_ONLY(x)
  #define DEBUG_PRINT(x)  BlackBoxAppend(x)
  #define DEBUG_PRINTLN(x)  BlackBoxAppendln(x)
  #define DEBUG_PRINTLN2(x,y)  BlackBoxAppendln(x,y)
  #define DEBUG_PRINTLINE() BlackBoxAppendln("------------------------------------");
  #define DEBUG_WRITE(x)
  #define RECEIVE_CMDS()  recvWithStartEndMarkers()
#endif

#define ToBoolString(m) ((m) == 1 ? "T" : ((m) == 0 ? "F" : "Unknown"))
// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //
#define SystemVersion   "0.1.1"

// Binary codes for Status LED flags, Red Yellow Green
#define BINARY_CODE_LED_GRN B001   // 001
#define BINARY_CODE_LED_YEL B010   // 010
#define BINARY_CODE_LED_RED B100   // 100

// LED blink durations (ms)
#define LED_BLINK_VERY_LONG     2000
#define LED_BLINK_LONG          1000
#define LED_BLINK_SHORT         250
#define LED_BLINK_VERY_SHORT    100

// Button debounce
#define BUTTON_DBOUNCE_TIME     200
#define BTN_DEBOUNCE_TIME_LONG  2500

// Sensor and Module status
#define SYSTEM_CHECK_DT       1000
#define SYSTEM_REBOOT_DT      5000

#define MODULE_COUNT          16

#define MODULE_PWR_PRIMARY    0
#define MODULE_PWR_SECONDARY  1
#define MODULE_PWR_MOTOR      2
#define MODULE_PWR_CANISTER   3
#define MODULE_MOTOR          4
#define MODULE_SD             5
#define MODULE_CLOCK          6
#define MODULE_NA2            7
#define MODULE_NA3            8
#define MODULE_NA4            9
#define MODULE_NA5            10
#define MODULE_COMM_LORA      11
#define MODULE_COMM_CANISTER  12
#define MODULE_COMM_DBG       13
#define MODULE_BLACKBOX       14
#define MODULE_RESERVED       15


// Requirements to run a sample
const unsigned long SYSREQ_SAMPLE =  \
(1L << MODULE_PWR_PRIMARY)    + \
(1L << MODULE_PWR_MOTOR)      + \
(1L << MODULE_PWR_CANISTER)   + \
(1L << MODULE_MOTOR)          + \
(1L << MODULE_SD)             + \
(1L << MODULE_COMM_CANISTER);


// ------------------------------------------------------------ //
//                       STRATEGY MODES                         //
// ------------------------------------------------------------ //

#define MODES_MAX           4 // Total number of modes
#define MODES_MIN_BROWSABLE 1 // Minimum mode index, that could be set via mode button.

#define MODE_SYSTEMTEST     0 // Test main systems 
#define MODE_IDLE           1 // Standby mode
#define MODE_AUTONOMOUS     2 // Autonomous driving mode
#define MODE_SERVICE        3 // Service mode

#define ModeToString(m) ((m) == 0 ? "Systesm Test" : ((m) == 1 ? "Idle" : ((m) == 2 ? "Autonomous" : \
((m) == 3 ? "Service" : "Unknown"))))


// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT     1       // times per minute
#define HRTBEAT_DT_OUT      60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD    300000  // Treshold for late heartbeat

#define BACKUP_RST_DT       300000  // Time between each reset attempt

#define HRTBEAT_DT_LOG      300000  // Time between system status log



// ------------------------------------------------------------ //
//                           MOTORS                             //
// ------------------------------------------------------------ //

#define MOTOR_SPEED             100
#define MOTOR_ACCEL             20
#define MOTOR_STEPS             5
#define MOTOR_POS_TOP           500
#define MOTOR_POS_BOT           -500
#define MOTOR_DIR_UP            1
#define MOTOR_DIR_DOWN          2
#define MOTOR_DIR_HALT          0



// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME       500
#define ESTOP_DEBOUNCE_TIME     1000


// DEBUG
#define DEBUG_BAUDRATE          115200

// Canister Comm (RS232)
#define CANISTER_BAUDRATE       115200


// ------------------------------------------------------------ //
//                           EEPROM                             //
// ------------------------------------------------------------ //
#define EEPROM_READ_INT(addr) (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr+1) << 8) & 0xFF00))

// Modes
#define MEMADDR_LASTMODE 0
// motor calibration cache


// ------------------------------------------------------------ //
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK          '<'
#define CMD_END_MARK            '>'

#define CMD_FILES               'F'
#define CMD_FILES_LIST          'L'
#define CMD_FILES_SIZE          'S'
#define CMD_FILES_DOWNLOAD      'D'
#define CMD_FILES_DELETE        'R'

#define CMD_STRATEGY            'S'
#define CMD_STRATEGY_SET        'S'
#define CMD_STRATEGY_FUNCTION   'F'
#define CMD_STRATEGY_OVERRIDE   'O'

#define CMD_BACKUP              'B'
#define CMD_BACKUP_RST          'R'
#define CMD_BACKUP_PRIMSTATUS   'S'
#define CMD_BACKUP_HB           'H'

#define CMD_MODULE              'M'
#define CMD_MODULE_ENABLE       'E'
#define CMD_MODULE_DISABLE      'D'
#define CMD_MODULE_OVERRIDE     'O'
#define CMD_MODULE_STATUS       'S'
#define CMD_MODULE_RESET        'R'

