
/*
    Lora communication (RFM9X), handles reading serial and parsing commands.

    Primary Library:
      https://github.com/PaulStoffregen/RadioHead

    Component:
      https://www.seeedstudio.com/Grove-LoRa-Radio-868MHz.html

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/
#pragma once

#include "../setup/modules.h"

// Library
#include <RH_RF95.h>

// TODO: rewrite to reliabile version https://github.com/Seeed-Studio/Grove_LoRa_433MHz_and_915MHz_RF/blob/master/examples/rf95_reliable_datagram_client/rf95_reliable_datagram_client.ino
RH_RF95 rf95(COM_SERIAL_LORA);

bool InitializeLora() {
  bool status = rf95.init();
  if (status) {
    rf95.setFrequency(LORA_FREQUENCY);
    rf95.setModeIdle();
  }
  return status;
}

bool LoraStatus() {
  return GetStatus(MODULE_COMM_LORA);
}

void LoraSendMsg(uint8_t data[], uint8_t size) {
  rf95.send(data, size);
  rf95.waitPacketSent();
}

void LoraRecMsg() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.recv(buf, &len)) {
    DEBUG_PRINTLN((char*)buf);
  }
}

// TODO: test termination proces
void TerminateLora() {
  rf95.sleep();
}