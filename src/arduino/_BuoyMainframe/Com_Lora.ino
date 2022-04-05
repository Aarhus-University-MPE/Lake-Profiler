/*
    Lora communication (RFM9X), handles reading serial and parsing commands.

    https://github.com/PaulStoffregen/RadioHead

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#include <RH_RF95.h>

// TODO: rewrite to uart version
RH_RF95 rf95(COM_SERIAL_LORA);

bool InitializeLora(){
  bool status = rf95.init();
  if(status){
    rf95.setFrequency(LORA_FREQUENCY);
  }
  return status;
}

bool LoraStatus(){
  return true;
}

void LoraSendMsg(uint8_t data[], uint8_t size){
  rf95.send(data, size);
  rf95.waitPacketSent();
}

void LoraRecMsg(){
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if(rf95.recv(buf, &len)){
    DEBUG_PRINTLN((char*)buf);
  }

}