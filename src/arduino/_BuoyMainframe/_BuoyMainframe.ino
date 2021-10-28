/*
  Lake Profiler mainframe
  - Manages primary systems

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021s
*/

#include "_constants.h"
#include "_pinout.h"
#include "_shared.h"


// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup()
{
  InitBlackBox();
  SetStatus(false);

  // Debug
  DBG_ONLY(initializeDebugComm());

  // System initialization
  InitAllPins();
  InitButtons();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();

}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop()
{
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartbeatBlackBox();

  DBG_ONLY(recvWithStartEndMarkers());
}
