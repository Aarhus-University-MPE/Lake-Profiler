
// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  Wire.begin();
  Wire.setWireTimeout(1000);

  // Heartbeat
  pinMode(LED_BUILTIN, OUTPUT);
}
