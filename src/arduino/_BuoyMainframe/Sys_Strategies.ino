// Set pointers for strategies methods
void InitStrategyMethods()
{
  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;
  strategyMethods[3][MODE_SYSTEMTEST] = SelectFunctionSystemTest;

  strategyMethods[0][MODE_IDLE] = StartStrategyIdle;
  strategyMethods[1][MODE_IDLE] = RunStrategyIdle;
  strategyMethods[2][MODE_IDLE] = FinishStrategyIdle;
  strategyMethods[3][MODE_IDLE] = SelectFunctionIdle;

  strategyMethods[0][MODE_AUTONOMOUS] = StartStrategyAutonomous;
  strategyMethods[1][MODE_AUTONOMOUS] = RunStrategyAutonomous;
  strategyMethods[2][MODE_AUTONOMOUS] = FinishStrategyAutonomous;
  strategyMethods[3][MODE_AUTONOMOUS] = SelectFunctionAutonomous;

  strategyMethods[0][MODE_SERVICE] = StartStrategyService;
  strategyMethods[1][MODE_SERVICE] = RunStrategyService;
  strategyMethods[2][MODE_SERVICE] = FinishStrategyService;
  strategyMethods[3][MODE_SERVICE] = SelectFunctionService;
}

// Set last mode from EEPROM
void InitMode()
{
  if(!digitalRead(PI_BUTTON_MODE_SERVICE)){
    mode = MODE_SERVICE;
  }
  else if(!digitalRead(PI_BUTTON_MODE_AUTONOMOUS))
  {
    mode = MODE_AUTONOMOUS;
  }
  else
  {
    mode = MODE_IDLE;
  }
  
  strategyMethods[0][mode]();
}

// Reads mode selector pins 
byte ModeRead(){
  byte readMode;
  if(!digitalRead(PI_BUTTON_MODE_SERVICE)){
    readMode = MODE_SERVICE;
  }
  else if(!digitalRead(PI_BUTTON_MODE_AUTONOMOUS))
  {
    readMode = MODE_AUTONOMOUS;
  }
  else
  {
    readMode = MODE_IDLE;
  }

  return readMode;
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater()
{
  if(mode != ModeRead()){
    SetMode(ModeRead());
  }
  while (isModeUpdated)
  {
    isModeUpdated = false;
    
    strategyMethods[2][prevMode](); // finish any operations for prevMode here
    
    strategyMethods[0][mode](); // init new strategy according to the new mode value
  }
}

// Tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode)
{
  if (newMode < MODES_MAX)
  {
    prevMode = mode;
    mode = newMode;
    isModeUpdated = true;
    EEPROM.write(MEMADDR_LASTMODE, mode);
    DEBUG_PRINT("Mode is set to: ");
    DEBUG_PRINTLN(ModeToString(mode));
    return true;
  }
  return false;
}
