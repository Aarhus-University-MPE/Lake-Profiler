
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
// #include <RHReliableDatagram.h>
// #include <RH_RF95.h>

// RH_RF95<HardwareSerial> rf95(COM_SERIAL_LORA);
// RHReliableDatagram manager(driver, CLIENT_ADDRESS);

bool InitializeLora() {
  // bool status = rf95.init();
  // if (status) {
  //   rf95.setFrequency(LORA_FREQUENCY);
  //   rf95.setModeIdle();
  // }
  // return status;
  return false;
}

bool LoraStatus() {
  return GetStatus(MODULE_COMM_LORA);
}

void LoraSendMsg(uint8_t data[], uint8_t size) {
  // DEBUG_PRINT(F("Sending msg to server.. "));
  // if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
  //   DEBUG_PRINTLN(F(" Success"));
  //   uint8_t len = sizeof(buf);
  //   uint8_t from;

  //   if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
  //     DEBUG_PRINT("got reply from : 0x");
  //     DEBUG_PRINT2(from, HEX);
  //     DEBUG_PRINT(": ");
  //     DEBUG_PRINTLN((char*)buf);
  //   } else {
  //     DEBUG_PRINTLN("No reply from server.");
  //   }
  // } else {
  //   DEBUG_PRINTLN(F(" Failed"));
  // }
  // rf95.waitPacketSent();
}

void LoraRecMsg() {
  // uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);

  // if (rf95.recv(buf, &len)) {
  //   DEBUG_PRINTLN((char*)buf);
  // }
}

void TerminateLora() {
  // rf95.sleep();
}