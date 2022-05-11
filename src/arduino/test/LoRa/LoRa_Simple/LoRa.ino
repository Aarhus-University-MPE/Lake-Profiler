/*
  Lora Test Sketch
*/

// Library
#include <RH_RF95.h>

// TODO: rewrite to reliabile version https://github.com/Seeed-Studio/Grove_LoRa_433MHz_and_915MHz_RF/blob/master/examples/rf95_reliable_datagram_client/rf95_reliable_datagram_client.ino
RH_RF95<HardwareSerial> rf95(Serial1);

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing Lora... "));
  while (!InitializeLora()) {
    Serial.println(F("Initialization failed!"));
    delay(5000);
    Serial.print(F("Initializing Lora... "));
  }
  Serial.println(F("Initialization Succes!"));
}

void loop() {
  LoraRecMsg();
}

bool InitializeLora() {
  bool status = rf95.init();
  if (status) {
    rf95.setFrequency(434.0f);
    rf95.setModeIdle();
  }
  return status;
}

void LoraSendMsg(uint8_t data[], uint8_t size) {
  rf95.send(data, size);
  rf95.waitPacketSent();
}

void LoraRecMsg() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.recv(buf, &len)) {
    Serial.println((char*)buf);
  }
}

// TODO: test termination proces
void TerminateLora() {
  rf95.sleep();
}