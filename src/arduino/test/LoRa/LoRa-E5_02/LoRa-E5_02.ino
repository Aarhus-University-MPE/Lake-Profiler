// LoRa E5 test

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  unsigned long startMillis = 0;
  int ch;
  bool recData = false;

  while (1) {
    Serial.println(F("LoRa Test... "));
    Serial1.print("AT\r\n");
    recData     = false;
    startMillis = millis();

    while (millis() - startMillis < 200 && !recData) {
      while (Serial1.available() > 0) {
        ch = Serial1.read();
        Serial.print((char)ch);
        delay(2);
        recData = true;
      }
    }
    if (!recData) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Succes!"));
    }
  }
}

void loop() {
}