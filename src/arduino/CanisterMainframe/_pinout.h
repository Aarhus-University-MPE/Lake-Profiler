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

// Communication
#define COM_SERIAL_PC Serial
#define COM_CH4       Serial1
#define COM_CO2       Serial2

#define COM_BAR100    Serial3
#define COM_LUM       A0
#define COM_TEMP      A1
