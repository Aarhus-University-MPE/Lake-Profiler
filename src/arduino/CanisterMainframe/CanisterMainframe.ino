/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "./modules/setup/modules.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();

  // Debug
  DBG_ONLY(SystemEnable(MODULE_COMM_DBG));
  DEBUG_PRINT(F("Debug mode. Entered Setup... "));

  // Setup finished
  DEBUG_PRINTLN(F("Setup complete."));

  SystemEnable(MODULE_BUOY_COMM);

  LoggingStart();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  DBG_ONLY(recvWithStartEndMarkersDebug());

  recvWithStartEndMarkers();  // Scan commands from Buoy

  // if (!systemActive) {
  //   BuoyCommHandshake();
  // }

  // if (systemActive) {
  SensorProcess();
  // }
  // DepthTest();
  // delay(2000);

  // TempTest();

  // delay(2000);

  // LumTest();
  // delay(2000);

  // parseDataCH4(149);
  // DEBUG_PRINT(F("CH4 Level: "));
  // DEBUG_PRINTLN(GetCH4Concentration());

  // delay(2000);

  // parseDataCO2(88);
  // DEBUG_PRINT(F("CO2 Level: "));
  // DEBUG_PRINTLN(GetCo2Concentration());
  // DEBUG_PRINTLINE();
}
