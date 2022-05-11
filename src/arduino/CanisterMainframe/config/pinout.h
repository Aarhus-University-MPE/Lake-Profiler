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

// Sensors
#define PA_LUM_SENSOR     A0

// Communication
#define COM_SERIAL_PC     Serial
#define COM_BUOY          Serial1
#define COM_CH4           Serial2
#define COM_CO2           Serial3

#define COM_BAR100        Serial3
#define COM_TEMP          A1

#define SENSOR_LOG_PERIOD 1000

#define COM_CH4_BAUDRATE  115200
#define COM_CO2_BAUDRATE  19200