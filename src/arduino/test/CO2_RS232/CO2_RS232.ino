/*
  Lake Profiler CO2 sensor communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing Communication... "));

  Serial3.begin(19200);

  Serial3.write(0x1b);  // Initiate Communication
  delay(50);
  Serial3.write(0x31);  // Sensor ON

  Serial.println(F("complete"));
}

unsigned long lastMillisSent = 0;
void loop() {
  while (Serial3.available() > 0) {
    // Serial.println("data");
    Serial.write(Serial3.read());
  }
}
