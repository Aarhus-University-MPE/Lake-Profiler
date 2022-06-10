// LoRa E5 test

void setup() {
  Serial.begin(115200);
  Serial3.begin(9600);

  unsigned long startMillis = 0;
  int ch;
  bool recData = false;

  while (1) {
    Serial.println(F("LoRa Test... "));
    Serial3.print("AT\r\n");
    recData     = false;
    startMillis = millis();

    while (millis() - startMillis < 200 && !recData) {
      while (Serial3.available() > 0) {
        ch = Serial3.read();
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