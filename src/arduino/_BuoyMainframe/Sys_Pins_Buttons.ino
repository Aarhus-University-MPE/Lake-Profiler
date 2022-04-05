// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // User Inputs
  pinMode(PI_SWITCH_MODE, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_UP, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_DOWN, INPUT_PULLUP);

  // Power control (Relays)
  digitalWrite(PO_POWER_CANISTER, LOW);
  digitalWrite(PO_POWER_SECONDARY, LOW);

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

  // Heartbeat
  digitalWrite(PO_CANISTER_HRTBEAT, true);
  digitalWrite(PO_CANISTER_RST, true);

  pinMode(PI_CANISTER_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_CANISTER_HRTBEAT, OUTPUT);
  pinMode(PO_CANISTER_RST, OUTPUT);

  InitInterrupts();
}

// Initialization of inputs
void InitInterrupts() {
  // Switch Mode interrupt
  attachInterrupt(PT_SWITCH_MODE, HeartBeatInInterrupt, CHANGE);

  // Encoder interrupts
  attachInterrupt(PI_ENCODER_A, HeartBeatInInterrupt, FALLING);
  attachInterrupt(PI_ENCODER_B, HeartBeatInInterrupt, FALLING);
  attachInterrupt(PI_ENCODER_Z, HeartBeatInInterrupt, FALLING);

  // Heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}