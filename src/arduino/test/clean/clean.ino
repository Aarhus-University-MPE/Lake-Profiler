/*
  Clean script
*/

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < 48; i++) {
    Serial.print(i);
    Serial.print((" "));
  }
  Serial.println();

  for (uint8_t i = 0; i < 48; i++) {
    Serial.print(HourToHex(i));
    Serial.print((" "));
  }
  Serial.println();

  for (uint8_t i = 0; i < 48; i++) {
    Serial.print(HexToHour(HourToHex(i)));
    Serial.print((" "));
  }
  Serial.println();
}

void loop() {
}

uint8_t HourToHex(uint8_t hour) {
  if (hour < 10) {
    return hour;
  } else if (hour < 20) {
    return hour + 6;
  } else if (hour < 24) {
    return hour + 12;
  } else {
    return HourToHex(hour - 24);
  }
}

uint8_t HexToHour(uint8_t hourHex) {
  if (hourHex < 10) {
    return hourHex;
  } else if (hourHex < 32) {
    return hourHex - 6;
  } else if (hourHex < 36) {
    return hourHex - 12;
  } else {
    return HexToHour(hourHex - 36);
  }
}