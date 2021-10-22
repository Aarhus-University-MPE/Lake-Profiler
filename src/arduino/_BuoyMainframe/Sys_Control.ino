/*
  GeoRover System control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

void SystemEnable(int module)
{
  if (GetStatus(module))
    return;

  bool status = true;

  switch (module)
  {
  case MODULE_PWR:
    status = BatteryStatus();
    break;
  case MODULE_PWR_MOTOR:
    digitalWrite(PO_POWER_MOTOR_ON, HIGH);
    delay(50);
    digitalWrite(PO_POWER_MOTOR_ON, LOW);
    DEBUG_PRINTLN("Power to Motors: Enabled");
    break;
  case MODULE_PWR_5V:
    digitalWrite(PO_POWER_5V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Enabled");
    break;
  case MODULE_PWR_12V:
    digitalWrite(PO_POWER_12V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems (12V): Enabled");
    break;
  case MODULE_RF:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_RF, HIGH);
    DEBUG_PRINT("Short Range Communication (RF): ");
    if (InitializeSBUS())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_IRIDIUM:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_IRIDIUM, HIGH);
    DEBUG_PRINT("Long Range Communication (Iridium): ");
    if (InitializeIridium())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_CANBUS:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("CanBus Communication: ");
    if (InitializeCanBus())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_MOTORS:
    SystemEnable(MODULE_PWR_MOTOR);
    DEBUG_PRINT("Motors: ");
    if (InitializeMotors(false))
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_ACCEL:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("Accelerometer: ");
    if (InitializeAccel())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
      DEBUG_PRINTLN("Error");
      status = false;
    break;
  case MODULE_GNSS:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("Global Navigation Satellite Systems: ");
    if (InitializeGnss())
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
    SystemEnable(MODULE_PWR_5V);
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
  default:
    break;
  }

  SetStatus(module, status);
  delay(10);
}

void SystemEnable()
{
  SystemEnable(MODULE_PWR_MOTOR);
  SystemEnable(MODULE_PWR_5V);
  SystemEnable(MODULE_PWR_12V);
  SystemEnable(MODULE_RF);
  SystemEnable(MODULE_IRIDIUM);
  SystemEnable(MODULE_GNSS);
  SystemEnable(MODULE_ACCEL);
  SystemEnable(MODULE_CANBUS);
  SystemEnable(MODULE_MOTORS);
  SystemEnable(MODULE_SD);
}

void SystemEnableMode(int mode){
  switch (mode)
  {
  case MODE_REMOTECONTROL:
    SystemEnable(MODULE_PWR);
    SystemEnable(MODULE_PWR_MOTOR);
    SystemEnable(MODULE_CANBUS);
    SystemEnable(MODULE_MOTORS);
    SystemEnable(MODULE_RF);
    break;
  case MODE_AUTONOMOUS:
    //SystemEnable();
    break;
  default:
    break;
  }
}

void SystemDisable(int module)
{
  if (!GetStatus(module))
    return;

  switch (module)
  {
  case MODULE_PWR_MOTOR:
    DEBUG_PRINTLN("Power to Motors: Disabled.");
    digitalWrite(PO_POWER_MOTOR_OFF, HIGH);
    delay(50);
    digitalWrite(PO_POWER_MOTOR_OFF, LOW);
    break;
  case MODULE_PWR_5V:
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Disabled");
    digitalWrite(PO_POWER_5V, LOW);
    break;
  case MODULE_PWR_12V:
    DEBUG_PRINTLN("Power to Secondary Systems (12V): Disabled");
    digitalWrite(PO_POWER_12V, LOW);
    break;
  case MODULE_RF:
    digitalWrite(PO_POWER_RF, LOW);
    TerminateSBUS();
    DEBUG_PRINTLN("Short Range Communication (RF): Disabled");
    break;
  case MODULE_IRIDIUM:
    digitalWrite(PO_POWER_IRIDIUM, LOW);
    DEBUG_PRINTLN("Long Range Communication (Iridium): Disabled");
    break;
  case MODULE_ACCEL:
    TerminateAccel();
    DEBUG_PRINTLN("Accelerometer: Disabled");
    break;
  case MODULE_GNSS:
    TerminateGnss();
    DEBUG_PRINTLN("Global Navigation Satellite Systems: Disabled");
    break;
  case MODULE_CANBUS:
    TerminateCanBus();
    DEBUG_PRINTLN("CanBus Communication: Disabled");
    break;
  case MODULE_MOTORS:
    TerminateMotors(false);
    DEBUG_PRINTLN("Motors: Disabled");
    break;
  default:
    break;
  }

  SetStatus(module, false);
}

void SystemDisable()
{
  SystemDisable(MODULE_RF);
  SystemDisable(MODULE_IRIDIUM);
  SystemDisable(MODULE_GNSS);
  SystemDisable(MODULE_ACCEL);
  SystemDisable(MODULE_MOTORS);
  SystemDisable(MODULE_CANBUS);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
}

bool SystemCheck(int mode){
  static bool status = false;
  SystemTest(false);
  
  switch (mode)
  {
  case MODE_REMOTECONTROL:
    status = (((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED);
    if(!status){
      DEBUG_PRINT("ERROR Code: ")
      DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << MODULE_RESERVED)));
    }
    break;
  case MODE_AUTONOMOUS:
    status = (((ToLong(SystemStatus) ^ SYSREQ_AUTONOMOUS) & SYSREQ_AUTONOMOUS) | (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED);
    if(!status){
      DEBUG_PRINT("ERROR Code: ")
      DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << MODULE_RESERVED)));
    }
    break;
  default:
    break;
  }

  return status;
}

// Run full system check
void SystemTest(bool printRes){
  // SetStatus(MODULE_PWR_MOTOR, digitalRead(PO_POWER_MOTOR_ON)); // Bi-stable relay, so not possible to measure
  SetStatus(MODULE_PWR_12V,   digitalRead(PO_POWER_12V));
  SetStatus(MODULE_PWR_5V,    digitalRead(PO_POWER_5V));
  SetStatus(MODULE_RF,       (digitalRead(PO_POWER_RF)      &&  digitalRead(PO_POWER_5V)   && SBusStatus()));
  SetStatus(MODULE_IRIDIUM,   IridiumTest(printRes));
  SetStatus(MODULE_PWR,       BatteryStatus());
  SetStatus(MODULE_MOTORS,    MotorStatus());
  SetStatus(MODULE_MOTOR_ACT, MotorState());
  SetStatus(MODULE_GNSS,      GnssTest(printRes));
  SetStatus(MODULE_SD,        SDReaderStatus());
  SetStatus(MODULE_ACCEL,     AccelTest(printRes));
  SetStatus(MODULE_DBGCOMM,   DebugCommStatus());
  SetStatus(MODULE_BACKUPCPU, HeartBeatStatus());
  SetStatus(MODULE_ESTOP,     EmergencyStopStatus());
  SetStatus(MODULE_BLACKBOX,  BlackBoxStatus());
  SetStatus(MODULE_RESERVED,  true);
}