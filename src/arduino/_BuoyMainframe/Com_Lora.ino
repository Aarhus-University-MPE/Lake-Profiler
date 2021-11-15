/*
    Lora communication (RFM9X), handles reading serial and parsing commands.

    https://github.com/PaulStoffregen/RadioHead

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

#include <RH_RF95.h>

RH_RF95 rf95(PO_SPISS_LORA)

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

void LoraSendMsg(uint8_t data[]){
  rf95.send(data,sizeof(data));
  rf95.waitPacketSent();
}

void LoraRecMsg(){
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sifeof(buf);

  if(rf95.recv(buf, &len)){
    DEBUG_PRINTLN((char*)buf);
  }

}