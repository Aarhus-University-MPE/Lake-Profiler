/*
  Lake Profiler Command List
  - Sent over Serial (USB)

  Every message is wrapped with start and end mark
    '<COMMAND>'

  Example:
    '<FL>' - Returns a list of all files on the local storage

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#pragma once

// Wrappers
#define CMD_START_MARK           '<'
#define CMD_END_MARK             '>'

// File system
#define CMD_FILES                'F'
#define CMD_FILES_LIST           'L'
#define CMD_FILES_SIZE           'S'
#define CMD_FILES_DOWNLOAD       'D'
#define CMD_FILES_CREATE         'C'
#define CMD_FILES_REMOVE         'R'
#define CMD_FILES_WRITE          'W'
#define CMD_FILES_WRITENEWLINE   'N'
#define CMD_FILES_QUIT           'Q'
#define CMD_FILES_BLCKBOX        'B'
#define CMD_FILES_BLCKBOXEMPTY   'E'

// Strategy functions
#define CMD_STRATEGY             'S'
#define CMD_STRATEGY_SET         'S'
#define CMD_STRATEGY_FUNCTION    'F'
#define CMD_STRATEGY_OVERRIDE    'O'

// System Modules
#define CMD_MODULE               'M'
#define CMD_MODULE_ENABLE        'E'
#define CMD_MODULE_DISABLE       'D'
#define CMD_MODULE_OVERRIDE      'O'
#define CMD_MODULE_STATUS        'S'
#define CMD_MODULE_RESET         'R'

// Realtime clock
#define CMD_CLOCK                'C'
#define CMD_CLOCK_SEC            's'
#define CMD_CLOCK_MIN            'm'
#define CMD_CLOCK_HOUR           'H'
#define CMD_CLOCK_DAY            'd'
#define CMD_CLOCK_DATE           'D'
#define CMD_CLOCK_MONTH          'M'
#define CMD_CLOCK_YEAR           'Y'
#define CMD_CLOCK_PRINT          'P'

// Realtime clock alarms
#define CMD_ALARM                'A'
#define CMD_ALARM_FREQUENCY      'F'
#define CMD_ALARM_START_HOUR     'S'
#define CMD_ALARM_BEGIN          'B'
#define CMD_ALARM_WARMUP_SET     'W'
#define CMD_ALARM_WARMUP_GET     'w'

// Motor Encoder
#define CMD_ENCODER              'E'
#define CMD_ENCODER_PRINT_POS    'P'
#define CMD_ENCODER_PRINT_TOP    'T'
#define CMD_ENCODER_PRINT_BOTTOM 'B'
#define CMD_ENCODER_SET_TOP      't'
#define CMD_ENCODER_SET_BOTTOM   'b'
#define CMD_ENCODER_SET_DEPTH    'D'  // Service depth in cm offset from 0 position (top logging position)
#define CMD_ENCODER_SET_SERVICE  'S'  // Service depth in cm offset from 0 position (top logging position)

// Power
#define CMD_POWER                'P'
#define CMD_POWER_SET            'S'
#define CMD_POWER_GET            'G'
#define CMD_POWER_PRINT          'P'

// LoggingOverride
#define CMD_LOGGING              'L'
#define CMD_LOGGING_START        'S'
#define CMD_LOGGING_BEGIN        'B'

// Canister
#define CMD_SAMPLE               's'
#define CMD_SAMPLE_IDGET         'I'
#define CMD_SAMPLE_IDSET         'i'

// ------------------------------------------------------------ //
//                      Example Commands                        //
// ------------------------------------------------------------ //

// Clock setup
// <Cm25><CH12><CD28><CM9><CY22><CP>

// Local storage Clear
// <FRdata/22051915.csv><FRdata/22051915.log>