/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

byte motorState;
int encoderCount;
byte motorDirection;

// Initialize motors and turns on encoder power
bool InitializeMotors() {
  digitalWrite(PO_POWER_ENCODER, true);

  // TODO: motor initialization...

  return true;
}

// Disables motors and turns off encoder power
void TerminateMotors() {
  digitalWrite(PO_POWER_ENCODER, false);

  // motor termination...
}

// Todo: test
unsigned long lastMillisRead;
void MotorProcess() {
  if (millis() - lastMillisRead < BUTTON_DBOUNCE_TIME) {
    return;
  }
  // TODO: DEBUG_PRINTLN(F("Reading... "));

  lastMillisRead = millis();
  if (!digitalRead(PI_BUTTON_MOTOR_UP)) {
    MotorMove(MOTOR_DIR_UP);
  } else if (!digitalRead(PI_BUTTON_MOTOR_DOWN)) {
    MotorMove(MOTOR_DIR_DOWN);
  } else {
    MotorMove(MOTOR_DIR_HALT);
  }
}

// move motors
void MotorMove(byte dir) {
  if (dir == motorState) {
    return;
  }
  motorState = dir;
  switch (dir) {
    case MOTOR_DIR_UP:
      // DEBUG_PRINTLN(F("Moving Up"));
      digitalWrite(LED_BUILTIN, true);
      StandbyMode();
      break;
    case MOTOR_DIR_DOWN:
      // DEBUG_PRINTLN(F("Moving Down"));
      RTCPrint();
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_HALT:
      // DEBUG_PRINTLN(F("Moving Halt"));
      digitalWrite(LED_BUILTIN, false);
      break;
    default:
      break;
  }
}

// Measure motor stall
bool MotorStall() {
  DEBUG_PRINTLN(F("Motor Stall!"));
  return false;
}

// // motor currently running?
// bool GetMotorState() {
//   return motorState;
// }

// Motors operational?
bool MotorStatus() {
  return GetStatus(MODULE_MOTOR) && GetStatus(MODULE_PWR_MOTOR);
}

// Increments encoder values
void EncoderAInterrupt() {
  motorDirection = digitalRead(PI_ENCODER_B);
  encoderCount += motorDirection;
}

// Increments encoder values
void EncoderBInterrupt() {
  // motorDirection = digitalRead(PI_ENCODER_A);
  // encoderCount += motorDirection;
}

int encoderRotations;
// Increments encoder values
void EncoderZInterrupt() {
  encoderRotations += motorDirection;
  encoderCount = 0;
}

// Save current position in EEPROM
void EEPROMSetMotorPos() {
  EEPROM_WRITE_INT(MEMADDR_ENCODER_COUNT, encoderCount);
  EEPROM_WRITE_INT(MEMADDR_ENCODER_ROTATION, encoderRotations);
}

// Read latest position in EEPROM
void EEPROMGetMotorPos() {
  encoderCount     = EEPROM_READ_INT(MEMADDR_ENCODER_COUNT);
  encoderRotations = EEPROM_READ_INT(MEMADDR_ENCODER_ROTATION);
}