
/*
    Lora communication (RFM9X), handles reading serial and parsing commands.

    Component:
      https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/
#pragma once

#include "../setup/modules.h"

static char recv_buf[512];
unsigned long lineRead = 0;

bool InitializeLora() {
  COM_SERIAL_LORA.begin(COM_LORA_BAUDRATE);

  if (!at_send_check_response(false, "+AT: OK", 1000, "AT\r\n")) return false;

  // Configure LoRa connection
  if (!LoRaConfigure()) return false;

  return true;
}

bool LoraStatus() {
  return GetStatus(MODULE_LORA);
}

// void LoraRecMsg() {
//   // uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//   // uint8_t len = sizeof(buf);

//   // if (rf95.recv(buf, &len)) {
//   //   DEBUG_PRINTLN((char*)buf);
//   // }
// }

void TerminateLora() {
  COM_SERIAL_LORA.end();
}

// Setup LoRa configurations
bool LoRaConfigure() {
  // LWOTAA (Over the Air Activation), LWABP (Activation By Personalization)
  if (!at_send_check_response(false, "+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n")) return false;

  // EU Band
  if (!at_send_check_response(false, "+DR: EU868", 1000, "AT+DR=EU868\r\n")) return false;

  // Channels
  if (!at_send_check_response(false, "+CH: NUM", 1000, "AT+CH=NUM,0-2\r\n")) return false;

  // Application Session Key
  if (!at_send_check_response(false, "+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"2B7B151628AED2A6AAF7157609CF4F2C\"\r\n")) return false;

  // LoRaWAN Class
  if (!at_send_check_response(false, "+CLASS: A", 1000, "AT+CLASS=A\r\n")) return false;

  // Broadcast Port
  if (!at_send_check_response(false, "+PORT: 8", 1000, "AT+PORT=8\r\n")) return false;

  return true;
}

// Connect to Network
bool LoRaJoin() {
  if (!LoraStatus()) return false;
  return at_send_check_response(false, "+JOIN: Network joined", 12000, "AT+JOIN\r\n");
}

static int at_send_check_response(bool printResponse, String p_ack_str, int timeout_ms, String p_cmd_str, ...) {
  int ch;
  int num                   = 0;
  int index                 = 0;
  unsigned long startMillis = 0;
  const char *p_ack         = p_ack_str.c_str();
  const char *p_cmd         = p_cmd_str.c_str();
  char buff[512];

  va_list args;
  memset(recv_buf, 0, sizeof(recv_buf));
  va_start(args, p_cmd_str);
  sprintf(buff, p_cmd, args);
  COM_SERIAL_LORA.print(buff);
  // Serial.print(F("Sending: "));
  // Serial.print(buff);
  va_end(args);
  delay(200);
  startMillis = millis();

  if (p_ack == NULL) {
    return 0;
  }

  do {
    while (COM_SERIAL_LORA.available() > 0) {
      ch                = COM_SERIAL_LORA.read();
      recv_buf[index++] = ch;
      if (printResponse) {
        DEBUG_PRINT((char)ch);
      }
      delay(2);
    }

    if (strstr(recv_buf, p_ack) != NULL) {
      return 1;
    }

  } while (millis() - startMillis < timeout_ms);
  return 0;
}

static void recv_prase(char *p_msg) {
  if (p_msg == NULL) {
    return;
  }
  char *p_start = NULL;
  int data      = 0;
  int rssi      = 0;
  int snr       = 0;

  p_start = strstr(p_msg, "RX");
  if (p_start && (1 == sscanf(p_start, "RX: \"%d\"\r\n", &data))) {
    Serial.println(data);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  p_start = strstr(p_msg, "RSSI");
  if (p_start && (1 == sscanf(p_start, "RSSI %d,", &rssi))) {
  }
  p_start = strstr(p_msg, "SNR");
  if (p_start && (1 == sscanf(p_start, "SNR %d", &snr))) {
  }
}

// Reset broadcast
void LoRaBroadcastReset() {
  lineRead = 0;
}

// Transmit log
void LoRaTransmitLog() {
  char cmd[128];
  sprintf(cmd, "AT+CMSGHEX=\"%d%d\"\r\n", (int)314, (int)314);
  Serial.print(cmd);
  Serial.println(F("Sending Message... "));
  int ret;
  ret = at_send_check_response(false, "Done", 5000, cmd);
  if (ret) {
    Serial.println(F("Message Sent!"));
    recv_prase(recv_buf);
  } else {
    Serial.print("Send failed!\r\n\r\n");
  }
  delay(5000);
}

bool LoRaInitializeBroadcastLog() {
  LoRaBroadcastReset();

  // Check File Integrity
  if (!LogFileLoad()) {
    DEBUG_PRINTLN(F("Log File Error!"));
    return false;
  }

  DEBUG_PRINTLN(F("Broadcasting Log"));
  return true;
}

bool LoRaInitializeBroadcastData() {
  LoRaBroadcastReset();

  // Check File Integrity
  if (!DataFileLoad()) {
    DEBUG_PRINTLN(F("Data File Error!"));
    return false;
  }

  DEBUG_PRINTLN(F("Broadcasting Data Initialized"));
  return true;
}

// Append package to cmd string
void LoRaBuildCommand(uint8_t package[30], char cmd[128]) {
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"");

  int i;

  for (i = 0; i < 3; i++) {
    if (i > 0) {
      cmdPtr += sprintf(cmdPtr, " ");
    }
    cmdPtr += sprintf(cmdPtr, "%02X", package[i]);
  }
  cmdPtr += sprintf(cmdPtr, "\"\n\r");
}

// Transmit latest log
bool LoRaBroadcastLog() {
  if (!LoraStatus()) {
    DEBUG_PRINTLN(F("LoRa Module Error!"));
    return true;
  }

  if (!GetStatus(MODULE_COMM_LORA)) {
    DEBUG_PRINTLN(F("LoRa Connection Error!"));
    return true;
  }

  uint8_t package[30];

  // Read line from log file
  bool endOfPackage = true;  // TODO: Remove
  // bool endOfPackage = LogReadLine(package);

  // Build Lora package
  char cmd[128];
  LoRaBuildCommand(package, cmd);

  // Send package over LoRa
  if (at_send_check_response(false, "Done", 5000, cmd)) lineRead++;  // TODO: limit to x retries

  return endOfPackage;
}

// Transmit latest data
bool LoRaBroadcastData() {
  if (!LoraStatus()) {
    DEBUG_PRINTLN(F("LoRa Module Error!"));
    return true;
  }

  if (!GetStatus(MODULE_COMM_LORA)) {
    DEBUG_PRINTLN(F("LoRa Connection Error!"));
    return true;
  }

  uint8_t package[30];

  // Read line from Data file
  bool endOfPackage = true;  // TODO: Remove
  // bool endOfPackage = DataReadLine(package);

  // Build Lora package
  char cmd[128];
  LoRaBuildCommand(package, cmd);

  // Send package over LoRa
  if (at_send_check_response(false, "Done", 5000, cmd)) lineRead++;  // TODO: limit to x retries

  return endOfPackage;
}
