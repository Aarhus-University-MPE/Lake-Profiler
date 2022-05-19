#pragma once

#include "./modules.h"

unsigned long lastMillisMode, lastMillisUp, lastMillisDown;

// Initialization of inputs
void InitInterrupts() {
  // Switch Mode interrupt
  attachInterrupt(PT_SWITCH_MODE, ModeSwitchInterrupt, CHANGE);

  // Encoder interrupts
  attachInterrupt(PT_ENCODER_A, EncoderAInterrupt, FALLING);  // Triggers 100 times per revolution (3.6° resolution)
  // attachInterrupt(PT_ENCODER_B, EncoderBInterrupt, FALLING);  // Triggers 100 times per revolution (3.6° resolution)
  attachInterrupt(PT_ENCODER_Z, EncoderZInterrupt, RISING);  // Triggers once every revolution
}

// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // User Inputs
  pinMode(PI_SWITCH_MODE, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_UP, INPUT_PULLUP);
  pinMode(PI_BUTTON_MOTOR_DOWN, INPUT_PULLUP);

  // Power control (Relays)
  digitalWrite(PO_POWER_CANISTER, LOW);
  digitalWrite(PO_POWER_ENCODER, HIGH);

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

  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  pinMode(16, OUTPUT);
  pinMode(17, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  Wire.setWireTimeout(1000);

  InitInterrupts();
}

// Check for Up Button noise, returns true if valid button press
bool UpButtonDebounce() {
  if (millis() - lastMillisUp < BTN_DEBOUNCE_TIME) {
    return false;
  }

  // Small time delay to filter voltage spikes
  delay(50);

  // Check if button is still pressed
  if (digitalRead(PI_BUTTON_MOTOR_UP)) {
    return false;
  }

  // Set timestamp for button debouncing
  lastMillisUp = millis();
  return true;
}

// Check for Down Button noise, returns true if valid button press
bool DownButtonDebounce() {
  if (millis() - lastMillisDown < BTN_DEBOUNCE_TIME) {
    return false;
  }

  // Small time delay to filter voltage spikes
  delay(50);

  // Check if button is still pressed
  if (digitalRead(PI_BUTTON_MOTOR_UP)) {
    return false;
  }

  // Set timestamp for button debouncing
  lastMillisDown = millis();
  return true;
}

// Check for Select Button noise, returns true if valid button press
bool ModeButtonDebounce() {
  if (millis() - lastMillisMode < BTN_DEBOUNCE_TIME) {
    return false;
  }

  // Small time delay to filter voltage spikes
  delay(50);

  bool currentMode = (mode == MODE_SERVICE);  // Signal High when service

  // Check if button is still at trigger signal position
  if (digitalRead(PI_SWITCH_MODE) == currentMode) {
    return false;
  }

  // Set timestamp for button debouncing
  lastMillisMode = millis();
  return true;
}
