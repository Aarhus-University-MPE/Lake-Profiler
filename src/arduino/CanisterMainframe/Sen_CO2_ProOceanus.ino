/*
    CO2 sensor (Pro Oceanus Mini CO2), handles reading sensor output.

    https://pro-oceanus.com/products/mini-series/mini-co2

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk
*/


bool CO2Initialize(){

  return true;
}

void CO2Terminate(){

}


bool CO2Status(){
  return GetStatus(MODULE_CO2);
}

bool CO2Test(){
  return true;
}


void CO2Read(){
  
}