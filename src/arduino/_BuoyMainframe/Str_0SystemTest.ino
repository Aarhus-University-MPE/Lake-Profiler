/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Starting");

  ModuleDisable();

  DEBUG_PRINTLN("Strategy (System Test): Initialized");
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  //
}


// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Ending");
  
  ModuleEnableMode();

  DEBUG_PRINTLN("Strategy (System Test): Finished");
}

// Strategy function
void SelectFunctionSystemTest(){
  DEBUG_PRINTLN("Running Full System Test");

  DEBUG_PRINTLN("  Enabling all systems");
  DEBUG_PRINTLINE();

  //ModuleEnable();
  DEBUG_PRINTLINE();
  
  DEBUG_PRINTLN("Testing subsystems");
  DEBUG_PRINTLINE();

  SystemTest(true);

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Disabeling all systems");
  DEBUG_PRINTLINE();

  unsigned long testResults = ToLong(SystemStatus);
  
  ModuleDisable();
  
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("System Test Complete");
  DEBUG_PRINT("  Results: ");
  DEBUG_PRINTLN(String(testResults));
  DEBUG_PRINTLINE();
}

