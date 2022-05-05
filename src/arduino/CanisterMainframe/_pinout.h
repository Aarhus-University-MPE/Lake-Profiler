/*
  GeoRover system pinout

  Naming pattern (all uppercase):
  P   + <I:input, A:analog input, O:output, P:pwm output>_
    + <general part name>_
    + <purpose>_
    + <where applicable: left, right, top, bottom, position etc...>

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// Heartbeat
#define PI_BUOY_HRTBEAT 2
#define PO_BUOY_HRTBEAT 3
#define PI_INT_HRTBEAT  digitalPinToInterrupt(PI_BUOY_HRTBEAT)

// Reset pin
#define PI_BUOY_RST     RESET
#define PO_BUOY_RST     4

// Communication
#define COM_SERIAL_PC   Serial
#define COM_CH4         Serial1
#define COM_CO2         Serial2
#define COM_CH4         Serial3
