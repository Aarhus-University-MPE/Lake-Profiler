/*  XX mode

    Description...
*/

// Start sequence of strategy
void StartStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINT("Strategy (Example): Emergency Starting...");

  // Disable all outputs
  ModuleDisable();
  delay(20);

  ModuleEnableMode(MODE_IDLE);

  DEBUG_PRINTLN("Strategy (Idle): Initialized");
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyExample() {
  //

}

// End sequence of strategy
void FinishStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Example): Ending");
  ModuleDisable();

  DEBUG_PRINTLN("Strategy (Example): Finished");
}