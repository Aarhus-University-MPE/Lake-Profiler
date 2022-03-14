// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // External Inputs
  pinMode(PI_BUTTON_MODE_AUTONOMOUS, INPUT_PULLUP);
  pinMode(PI_BUTTON_MODE_SERVICE, INPUT_PULLUP);

  // Power control (Relays)
  pinMode(PO_POWER_MOTOR, OUTPUT);
  pinMode(PO_POWER_SECONDARY, OUTPUT);
  pinMode(PO_POWER_CANISTER, OUTPUT);

  digitalWrite(PO_POWER_MOTOR, LOW);
  digitalWrite(PO_POWER_SECONDARY, LOW);
  digitalWrite(PO_POWER_CANISTER, LOW);

  // Heartbeat
  digitalWrite(PO_PRIMARY_HRTBEAT, true);
  digitalWrite(PO_PRIMARY_RST, true);

  pinMode(PI_PRIMARY_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_PRIMARY_HRTBEAT, OUTPUT);
  pinMode(PO_PRIMARY_RST, OUTPUT);

  // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}

// Initialization of buttons
void InitButtons() {
  //
}