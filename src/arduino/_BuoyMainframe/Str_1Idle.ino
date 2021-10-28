/*  Idle mode

    Strategy for Idle mode
    - Disables all systems
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Idle): Starting");

  // Disable all outputs
  ModuleEnableMode(MODE_IDLE);

  DEBUG_PRINTLN("Strategy (Idle): Initialized");
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyIdle() {
  //
}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Idle): Ending");
  ModuleDisable();

  DEBUG_PRINTLN("Strategy (Idle): Finished");
}

// Strategy function
void SelectFunctionIdle(){
  //
}