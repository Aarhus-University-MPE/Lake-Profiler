// **** INCLUDES *****
#include <LowPower.h>

// Use pin 2 as wake up pin
const int wakeUpPin = 3;

void wakeUp() {
  // Just a handler for the pin interrupt.
}

void setup() {
  // Configure wake up pin as input.
  // This will consumes few uA of current.
  pinMode(wakeUpPin, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, LOW);
}

void loop() {
  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  if (digitalRead(wakeUpPin)) {
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  }

  digitalWrite(LED_BUILTIN, true);
  delay(250);

  // Disable external pin interrupt on wake up pin.
  digitalWrite(LED_BUILTIN, false);
  delay(250);

  // Do something here
  // Example: Read sensor, data logging, data transmission.
}