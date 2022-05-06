void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}
unsigned long lastMillisPrint = 0;
void loop() {
  if (millis() - lastMillisPrint < 1000) {
    return;
  }
  lastMillisPrint = millis();

  Serial.print("A0 Read: ");
  Serial.println(analogRead(A0));
}