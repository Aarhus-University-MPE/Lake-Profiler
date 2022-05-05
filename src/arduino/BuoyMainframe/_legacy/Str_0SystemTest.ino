/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Starting"));

  ModuleDisable();

  DEBUG_PRINTLN(F("Strategy (System Test): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  //
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Ending"));

  ModuleDisable();
  ModuleEnableMode();

  DEBUG_PRINTLN(F("Strategy (System Test): Finished"));
}

// Strategy function
void SelectFunctionSystemTest() {
  DEBUG_PRINTLN(F("Running Full System Test"));

  DEBUG_PRINTLN(F("  Enabling all systems"));
  DEBUG_PRINTLINE();

  // ModuleEnable();
  DEBUG_PRINTLINE();

  DEBUG_PRINTLN(F("Testing subsystems"));
  DEBUG_PRINTLINE();

  SystemTest(true);

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Disabeling all systems"));
  DEBUG_PRINTLINE();

  unsigned long testResults = ToLong(SystemStatus);

  ModuleDisable();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("System Test Complete"));
  DEBUG_PRINT(F("  Results: "));
  DEBUG_PRINTLN(String(testResults));
  DEBUG_PRINTLINE();
}
