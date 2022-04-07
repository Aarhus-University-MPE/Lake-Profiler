/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <AccelStepper.h>
// AccelStepper stepper(MOTOR_STEPS, PO_MOTOR_P1, PO_MOTOR_P2);
byte motorState;

bool InitializeMotors() {
  // set the speed of the motor to 30 RPMs
  // stepper.setMaxSpeed(MOTOR_SPEED);
  // stepper.setAcceleration(MOTOR_ACCEL);

  return true;
}

void TerminateMotors() {
  //
}

unsigned long lastMillisRead;
void MotorProcess() {
  if(millis() - lastMillisRead < BUTTON_DBOUNCE_TIME){
    return;
  }
  // DEBUG_PRINTLN(F("Reading... "));
  
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
      DEBUG_PRINTLN(F("Moving Up"));
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_DOWN:
      DEBUG_PRINTLN(F("Moving Down"));
      digitalWrite(LED_BUILTIN, true);
      break;
    case MOTOR_DIR_HALT:
      DEBUG_PRINTLN(F("Moving Halt"));
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
