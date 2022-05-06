/*
  GeoRover Backup CPU system constants

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// ------------------------------------------------------------ //
//                            DEBUG                             //
// ------------------------------------------------------------ //

// Debug configuration flag - Comment out to unset
// #define DEBUG_BLACKBOX_AND_PRINT
// #define DEBUG_BLACKBOX
#define DEBUG_SERIAL
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

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT     6  // times per minute
#define HRTBEAT_DT_OUT      60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD    60000

#define BACKUP_RST_FRQ      2
#define BACKUP_RST_DT       60000 / BACKUP_RST_FRQ

// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME   300
#define HANDSHAKE_PERIOD    1000

// DEBUG
#define BUOY_BAUDRATE       115200
#define DEBUG_BAUDRATE      115200

// Sensor and Modules
#define MODULE_COUNT        7

#define MODULE_BUOY_COMM    0
#define MODULE_BUOY_HRTBEAT 1
#define MODULE_CH4          2
#define MODULE_CO2          3
#define MODULE_LUM          4
#define MODULE_DEPTH        5
#define MODULE_TEMP         6
#define MODULE_DEBUGCOMM    7
#define MODULE_RESERVED     8

#define ModuleToString(m) \
  ((m) == 0 ? "Buoy Communication" : ((m) == 1 ? "Buoy Heartbeat" : ((m) == 2 ? "CH4 Sensor" : ((m) == 3 ? "CO2 Sensor" : ((m) == 4 ? "Cyclops-7F Sensor" : ((m) == 5 ? "Depth Sensor" : ((m) == 6 ? "Temperature Sensor" : ((m) == 7 ? "Debug Communication" : ((m) == 8 ? "Reserved" : ("Unknown"))))))))))

// ------------------------------------------------------------ //
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK         '<'
#define CMD_END_MARK           '>'
#define CMD_SPLIT_MARK         ','

#define CMD_BACKUP             'B'
#define CMD_BACKUP_RST         'R'
#define CMD_BACKUP_BCKUPSTATUS 'S'
#define CMD_BACKUP_HB          'H'

#define CMD_LOGGING            'L'