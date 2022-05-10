/*  Autonomous mode

    Strategy for Autonomous mode
    - Starts logging procedure every specified interval
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting"));

  // Disable all outputs
  ModuleDisable();
  ModuleEnableMode();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  //
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending"));

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished"));
}

// Strategy function
void SelectFunctionAutonomous(){
  //
}