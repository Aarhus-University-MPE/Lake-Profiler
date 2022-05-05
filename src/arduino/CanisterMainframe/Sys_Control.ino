/*
  Lake Profiler System Control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/
unsigned long lastSystemCheck = 9999999;


void SystemEnable(int module) {
  if (GetStatus(module)) return;
  DEBUG_PRINT(F("SYS Enable: "));
  DEBUG_PRINT(ModuleToString(module));

  bool status = false;

  switch (module) {
    case MODULE_BUOY_COMM:
      status = BuoyCommInitialize();
      break;
    case MODULE_BUOY_HRTBEAT:
      status = HeartBeatStatus();
      break;
    case MODULE_CH4:
      status = CH4Initialize();
      break;
    case MODULE_CO2:
      status = CO2Initialize();
      break;
    case MODULE_LUM:
      status = LumInitialize();
      break;
    case MODULE_DEPTH:
      status = DepthInitialize();
      break;
    case MODULE_TEMP:
      status = TempInitialize();
      break;
    case MODULE_DEBUGCOMM:
      status = DebugCommInitialize();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      DEBUG_PRINT(F("- UNKNOWN Case"));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F(": Enabled"));
  } else {
    DEBUG_PRINTLN(F(": ERROR"));
  }
  SetStatus(module, status);
}

void SystemDisable(int module) {
  if (!GetStatus(module)) return;
  bool status = false;
  DEBUG_PRINT(F("SYS Disable: "));
  DEBUG_PRINT(ModuleToString(module));

  switch (module) {
    case MODULE_BUOY_COMM:
      BuoyCommTerminate();
      break;
    case MODULE_BUOY_HRTBEAT:
      status = HeartBeatStatus();
      break;
    case MODULE_CH4:
      CH4Terminate();
      break;
    case MODULE_CO2:
      CO2Terminate();
      break;
    case MODULE_LUM:
      LumTerminate();
      break;
    case MODULE_DEPTH:
      DepthTerminate();
      break;
    case MODULE_TEMP:
      TempTerminate();
      break;
    case MODULE_DEBUGCOMM:
      DebugCommTerminate();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      DEBUG_PRINT(F("- UNKNOWN Case"));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F("Unable to Disable"));
  } else {
    DEBUG_PRINTLN(F("- Disabled"));
  }

  SetStatus(module, status);
}

// Disable all secondary systems
void SystemDisable() {
  SystemDisable(MODULE_CH4);
  SystemDisable(MODULE_CO2);
  SystemDisable(MODULE_LUM);
  SystemDisable(MODULE_DEPTH);
}

// Run full system test
bool SystemTest() {
  unsigned long testResults;
  static int systemTestState;
  bool testDone = false;
  switch (systemTestState) {
    case 0:
      DEBUG_PRINTLN(F("Running Full System Test"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 1:
      DEBUG_PRINTLN(F("Running test (1/5) - CH4 Sensor (Contros HydroC CH4)"));
      systemTestState++;
      break;
    case 2:
      if(SystemTestModule(MODULE_CH4)) systemTestState++;
      break;
    case 3:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Running test (2/5) - CO2 Sensor (Pro Oceanus Mini CO2)"));
      systemTestState++;
    case 4:
      if (SystemTestModule(MODULE_CO2)) systemTestState++;
      break;
    case 5:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Running test (3/5) - Luminesence Sensor (Cyclops-7F)"));
      systemTestState++;
      break;
    case 6:
      if (SystemTestModule(MODULE_LUM)) systemTestState++;
      break;
    case 7:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Running test (4/5) - Depth Sensor (Blue Robotics Bar100)"));
      systemTestState++;
      break;
    case 8:
      if (SystemTestModule(MODULE_DEPTH)) systemTestState++;
      systemTestState++;
      break;
    case 9:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Running test (5/5) - Temperature Sensor (Blue Robotics TSYS01)"));
      break;
    case 10:
      if (SystemTestModule(MODULE_TEMP)) systemTestState++;
      break;
    case 11:
      DEBUG_PRINTLINE();
      break;
    case 12:
      testResults = ToLong(SystemStatus);
      DEBUG_PRINTLN(F("Disabeling all systems"));
      DEBUG_PRINTLINE();
      SystemDisable();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("System Test Complete"));
      DEBUG_PRINT(F("  Results: "));
      DEBUG_PRINTLN(String(testResults));
      DEBUG_PRINTLINE();
      testDone        = true;
      systemTestState = 0;
      break;
    default:
      DEBUG_PRINTLN(F("System Test Error: Stopping"));
      systemTestState = 0;
      testDone        = true;
      break;
  }

  return testDone;
}

bool SystemTestModule(byte module) {
  SystemEnable(module);
  bool status   = false;
  bool testDone = true;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_BUOY_COMM:
        testDone = BuoyCommTest();
        status   = BuoyCommStatus();
        break;
      case MODULE_BUOY_HRTBEAT:
        status = HeartBeatStatus();
        break;
      case MODULE_CH4:
        testDone = CH4Test();
        status   = CH4Status();
        break;
      case MODULE_CO2:
        testDone = CO2Test();
        status   = CO2Status();
        break;
      case MODULE_LUM:
        testDone = LumTest();
        status   = LumStatus();
        break;
      case MODULE_DEPTH:
        testDone = DepthTest();
        status   = DepthStatus();
        break;
      case MODULE_TEMP:
        testDone = TempTest();
        status   = TempStatus();
        break;
      case MODULE_DEBUGCOMM:
        status = DebugCommStatus();
        break;
      case MODULE_RESERVED:
        status = true;
        break;
      default:
        DEBUG_PRINT(F("- UNKNOWN Case"));
        break;
    }
  }

  SetStatus(module, status);

  return testDone;
}

// Checks all Systems
void SystemCheck() {
  for (int i = 0; i < MODULE_COUNT - 1; i++) {
    SystemCheckModule(i);
  }

  SetStatus(MODULE_RESERVED, true);
}

bool SystemCheckModule(byte module) {
  bool status = false;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_BUOY_COMM:
        status   = BuoyCommStatus();
        break;
      case MODULE_BUOY_HRTBEAT:
        status = HeartBeatStatus();
        break;
      case MODULE_CH4:
        status   = CH4Status();
        break;
      case MODULE_CO2:
        status   = CO2Status();
        break;
      case MODULE_LUM:
        status   = LumStatus();
        break;
      case MODULE_DEPTH:
        status   = DepthStatus();
        break;
      case MODULE_DEBUGCOMM:
        status = DebugCommStatus();
        break;
      case MODULE_RESERVED:
        status = true;
        break;
      default:
        DEBUG_PRINT(F("- UNKNOWN Case"));
        break;
    }
  }

  SetStatus(module, status);
  return status;
}
