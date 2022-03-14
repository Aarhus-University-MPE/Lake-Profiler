/*
  GeoRover System control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// Enable individual module
void ModuleEnable(int module)
{
  if (GetStatus(module))
    return;

  bool status = true;

  switch (module)
  {
  case MODULE_PWR_PRIMARY:
    status = BatteryStatus();
    break;
  case MODULE_PWR_SECONDARY:
    DEBUG_PRINTLN("Power (Secondary): Enabled");
    break;
  case MODULE_PWR_MOTOR:
    DEBUG_PRINTLN("Power (Motor): Enabled");
    break;
  case MODULE_PWR_CANISTER:
    DEBUG_PRINTLN("Power (Canister): Enabled");
    break;
  case MODULE_MOTOR:
    DEBUG_PRINT("Motor Controller: ");
    if (InitializeMotors())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_SD:
    DEBUG_PRINT("Local Storage: ");
    if (InitializeSDReader())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_CLOCK:
    DEBUG_PRINT("Real Time Clock: ");
    if (InitializeRTC())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_COMM_LORA:
    DEBUG_PRINT("Communication (LORA): ");
    if (InitializeLora())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_COMM_CANISTER:
    DEBUG_PRINT("Communication (Canister): ");
    if (InitializeCanister())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;  
  default:
    DEBUG_PRINTLN("Unknown Module");
    break;
  }

  SetStatus(module, status);
  delay(10);
}

// Enable systems for specified mode
void ModuleEnableMode(){
  ModuleDisable(); // Disable secondary systems
  switch (mode)
  {
  case MODE_SYSTEMTEST:
    break;
  case MODE_IDLE:
    break;
  case MODE_AUTONOMOUS:
    ModuleEnable(MODULE_COMM_LORA);
    ModuleEnable(MODULE_CLOCK);
    break;
  case MODE_SERVICE:
    ModuleEnable(MODULE_PWR_MOTOR);
    ModuleEnable(MODULE_MOTOR);
    break;
  default:
    break;
  }
}

// Disables individual module
void ModuleDisable(int module)
{
  if (!GetStatus(module))
    return;

  switch (module)
  {
  case MODULE_PWR_SECONDARY:
    DEBUG_PRINTLN("Power (Secondary): Terminated");
    break;
  case MODULE_PWR_MOTOR:
    DEBUG_PRINTLN("Power (Motor): Terminated");
    break;
  case MODULE_PWR_CANISTER:
    DEBUG_PRINTLN("Power (Canister): Terminated");
    break;
  case MODULE_MOTOR:
    DEBUG_PRINT("Motor Controller: Terminated");
    break;
  case MODULE_COMM_LORA:
    DEBUG_PRINT("Communication (LORA): Terminated");
    break;
  case MODULE_COMM_CANISTER:
    DEBUG_PRINT("Communication (Canister): Terminated");
    break;  
  default:
    DEBUG_PRINTLN("Unknown Module");
    break;
  }


  SetStatus(module, false);
}

// Disables secondary systems
void ModuleDisable()
{
  ModuleDisable(MODULE_PWR_SECONDARY);
  ModuleDisable(MODULE_PWR_MOTOR);
  ModuleDisable(MODULE_PWR_CANISTER);
  ModuleDisable(MODULE_MOTOR);
  ModuleDisable(MODULE_COMM_LORA);
}

// Runs system check and compares active modules to required
bool SystemCheck(int mode){
  static bool status = false;
  SystemTest(false);
  
  switch (mode)
  {
  case MODE_AUTONOMOUS:
    status = (((ToLong(SystemStatus) ^ SYSREQ_SAMPLE) & SYSREQ_SAMPLE) | (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED);
    if(!status){
      DEBUG_PRINT("ERROR Code: ")
      DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_SAMPLE) & SYSREQ_SAMPLE) | (1L << MODULE_RESERVED)));
    }
    break;
  default:
    break;
  }

  return status;
}

// Run full system check
void SystemTest(bool printRes){
  SetStatus(MODULE_PWR_PRIMARY,     BatteryStatus());
  SetStatus(MODULE_PWR_SECONDARY,   digitalRead(PO_POWER_SECONDARY));
  SetStatus(MODULE_PWR_MOTOR,       digitalRead(PO_POWER_MOTOR));
  SetStatus(MODULE_PWR_CANISTER,    digitalRead(PO_POWER_MOTOR));
  SetStatus(MODULE_MOTOR,           GetMotorState());
  SetStatus(MODULE_SD,              SDReaderStatus());
  SetStatus(MODULE_COMM_LORA,       LoraStatus());
  SetStatus(MODULE_COMM_CANISTER,   CanisterCommStatus());
  SetStatus(MODULE_COMM_DBG,        DebugCommStatus());
  SetStatus(MODULE_BLACKBOX,        BlackBoxStatus());
  SetStatus(MODULE_RESERVED,        true);
}