#pragma once

// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // User Inputs
  pinMode(PI_SWITCH_MODE, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_UP, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_DOWN, INPUT_PULLUP);

  // Power control (Relays)
  digitalWrite(PO_POWER_CANISTER, LOW);
  digitalWrite(PO_POWER_ENCODER, LOW);

  pinMode(PO_POWER_CANISTER, OUTPUT);
  pinMode(PO_POWER_ENCODER, OUTPUT);

  // Battery
  pinMode(PA_SENSOR_BATT, INPUT);

  // Encoder
  pinMode(PI_ENCODER_A, INPUT_PULLUP);
  pinMode(PI_ENCODER_B, INPUT_PULLUP);
  pinMode(PI_ENCODER_Z, INPUT_PULLUP);

  // Motor
  digitalWrite(PO_MOTOR_UP, LOW);
  digitalWrite(PO_MOTOR_DOWN, LOW);

  pinMode(PO_MOTOR_UP, OUTPUT);
  pinMode(PO_MOTOR_DOWN, OUTPUT);

  // Communication
  pinMode(PO_SPISS_SDCARD, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  InitInterrupts();
}

// Initialization of inputs
void InitInterrupts() {
  // Switch Mode interrupt
  attachInterrupt(PT_SWITCH_MODE, ModeSwitchInterrupt, CHANGE);

  // Encoder interrupts
  attachInterrupt(PI_ENCODER_A, EncoderAInterrupt, FALLING);  // Triggers 100 times per revolution (3.6° resolution)
  // attachInterrupt(PI_ENCODER_B, EncoderBInterrupt, FALLING);  // Triggers 100 times per revolution (3.6° resolution)
  attachInterrupt(PI_ENCODER_Z, EncoderZInterrupt, FALLING);  // Triggers once every revolution
}