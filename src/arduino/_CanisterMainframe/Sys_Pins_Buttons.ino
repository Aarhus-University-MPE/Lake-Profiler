
// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins()
{
  Wire.begin();
  
  // Heartbeat
  digitalWrite(PO_BUOY_HRTBEAT, true);
  digitalWrite(PO_BUOY_RST, true);

  pinMode(PI_BUOY_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_BUOY_HRTBEAT, OUTPUT);
  pinMode(PO_BUOY_RST,     OUTPUT);

   // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}
