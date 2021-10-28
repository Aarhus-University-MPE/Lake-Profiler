/*
  Lake Profiler Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <AccelStepper.h>
AccelStepper stepper(MOTOR_STEPS, PO_MOTOR_P1, PO_MOTOR_P2);
bool motorState = false;

bool InitializeMotors()
{
  // set the speed of the motor to 30 RPMs
  stepper.setMaxSpeed(MOTOR_SPEED);
  stepper.setAcceleration(MOTOR_ACCEL);

  if (MotorStatus())
  {
    return true;
  }
  else
  {
    return false;
  }
}

void TerminateMotors()
{
  //
}

// Move motors
// dir: true = up, false = down
void MotorSet(byte dir)
{
  motorState = true;
  SetStatus(MODULE_MOTOR, true);
  switch (dir)
  {
  case MOTOR_DIR_UP:
    stepper.moveTo(MOTOR_POS_TOP);
    break;
  case MOTOR_DIR_DOWN:
    stepper.moveTo(MOTOR_POS_BOT);
    break;
  case MOTOR_DIR_HALT:
    stepper.stop();
  default:
    break;
  }
}

// move motors
void MotorMove()
{
  if (motorState){
    stepper.run();
    if(stepper.distanceToGo() == 0 || MotorStall()){
      motorState = false;
      SetStatus(MODULE_MOTOR, false);
    }
  }  
}

// Measure motor stall
bool MotorStall(){
  DEBUG_PRINTLN("Motor Stall!");
  return false;
}

// motor currently running?
bool GetMotorState()
{
  return motorState;
}

bool MotorStatus()
{
  bool valid = true;

  // Motors operational?

  return valid;
}
