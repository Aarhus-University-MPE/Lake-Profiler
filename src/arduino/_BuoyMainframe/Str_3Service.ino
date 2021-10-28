/*  Service mode

    Strategy for Service mode.
    - Disables logging 
    - Activates motor for surfacing Cansiter
*/

// Start sequence of strategy
void StartStrategyService() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Service): Starting");

  // Disable all secondary outputs
  ModuleEnableMode(MODE_SERVICE);

  MotorSet(MOTOR_DIR_UP);

  DEBUG_PRINTLN("Strategy (Service): Initialized");
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyService() {
  // Lift motor to surface
  MotorMove();
}

// End sequence of strategy
void FinishStrategyService() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Service): Ending");
  
  // Halt motor
  MotorSet(MOTOR_DIR_HALT);
  while(GetStatus(MODULE_MOTOR)){
    MotorMove();
  }

  DEBUG_PRINTLN("Strategy (Service): Finished");
}

// Strategy function
void SelectFunctionService(){
  //
}