
/*
    Lora communication (RFM9X), handles reading serial and parsing commands.

    Component:
      https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/

    Online Data:
      https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=6h
      https://prototype.asap-forecast.com/docs/#/default/get_logs__category_
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
  int ret = 0;

  // 3 attempts to connect to network
  while (ret < 3) {
    if (at_send_check_response(false, "+JOIN: Network joined", 12000, "AT+JOIN\r\n")) {
      return true;
    } else {
      DEBUG_PRINT(F(" failed, retrying.."));
    }
    ret++;
  }

  return false;
}

unsigned long millisLastLoraHeartbeat = -LORA_HEARTBEAT_PERIOD;
void LoRaHeartbeat() {
  if (LoggingActive()) return;

  if (!GetStatus(MODULE_COMM_LORA)) return;

  if (digitalRead(PO_MOTOR_DOWN) || digitalRead(PO_MOTOR_UP)) return;

  if (millis() - millisLastLoraHeartbeat < LORA_HEARTBEAT_PERIOD) return;

  millisLastLoraHeartbeat = millis();

  LoRaBroadcastPowerLevel();
}

// Send Low power message over LoRa, and next alarm hour
void LoRaBroadcastLowPower() {
  if (!LoraStatus()) return;
  if (!GetStatus(MODULE_COMM_LORA)) return;

  char cmd[128];
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"2 %02X %02X %02X\"\r\n", BatteryLevelHex(), BatteryVoltageHex(), NextAlarm());

  DEBUG_PRINTLN(F("Sending package: "));
  DEBUG_PRINT(cmd);

  int ret = 0;
  while (ret < 5 && !at_send_check_response(false, "RXWIN1", 10000, cmd)) {
    if (ret == 0) {
      DEBUG_PRINT(F("Failed, retrying attempt: "));
    }
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(ret + 1);
    DEBUG_PRINT(F("/5)..."));

    // Broadcast Delay
    delay(LORA_BROADCAST_DELAY);
    ret++;
  }
  if (ret == 5) {
    DEBUG_PRINTLN(F("Failed"));
  } else {
    DEBUG_PRINTLN(F("Success"));
  }
  DEBUG_PRINTLINE();
}

// Broadcast current power level
void LoRaBroadcastPowerLevel() {
  if (!LoraStatus()) return;
  if (!GetStatus(MODULE_COMM_LORA)) return;

  char cmd[128];
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"3 %02X %02X\"\r\n", BatteryLevelHex(), BatteryVoltageHex());

  DEBUG_PRINTLN(F("Sending package: "));
  DEBUG_PRINT(cmd);

  int ret = 0;
  while (ret < 5 && !at_send_check_response(false, "RXWIN1", 10000, cmd)) {
    if (ret == 0) {
      DEBUG_PRINT(F("Failed, retrying attempt: "));
    }
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(ret + 1);
    DEBUG_PRINT(F("/5)..."));

    // Broadcast Delay
    delay(LORA_BROADCAST_DELAY);
    ret++;
  }
  if (ret == 5) {
    DEBUG_PRINTLN(F("Failed"));
  } else {
    DEBUG_PRINTLN(F("Success"));
  }
  DEBUG_PRINTLINE();
}

// Send Position Error message over LoRa
void LoRaBroadcastPosError() {
  if (!LoraStatus()) return;
  if (!GetStatus(MODULE_COMM_LORA)) return;

  char cmd[128];
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"4\"\r\n");

  DEBUG_PRINTLN(F("Sending package: "));
  DEBUG_PRINT(cmd);

  int ret = 0;
  while (ret < 5 && !at_send_check_response(false, "RXWIN1", 10000, cmd)) {
    if (ret == 0) {
      DEBUG_PRINT(F("Failed, retrying attempt: "));
    }
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(ret + 1);
    DEBUG_PRINT(F("/5)..."));

    // Broadcast Delay
    delay(LORA_BROADCAST_DELAY);
    ret++;
  }
  if (ret == 5) {
    DEBUG_PRINTLN(F("Failed"));
  } else {
    DEBUG_PRINTLN(F("Success"));
  }
  DEBUG_PRINTLINE();
}

// Send Log begin over LoRa along with current time stamp
void LoRaBroadcastLogBegin() {
  if (!LoraStatus()) return;
  if (!GetStatus(MODULE_COMM_LORA)) return;

  char cmd[128];
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"1 %02X %02X ", BatteryLevelHex(), BatteryVoltageHex());

  // Current time
  unsigned long timeStamp = (unsigned long)now();

  // Convert timestamp to binary
  for (uint8_t i = 0; i < 4; i++) {
    if (i > 0) {
      cmdPtr += sprintf(cmdPtr, " ");
    }
    cmdPtr += sprintf(cmdPtr, "%02X", ((timeStamp >> (8 * i)) & 0XFF));
  }

  cmdPtr += sprintf(cmdPtr, "\"\r\n");

  DEBUG_PRINTLN(F("Sending package: "));
  DEBUG_PRINT(cmd);

  int ret = 0;
  while (ret < 5 && !at_send_check_response(false, "RXWIN1", 10000, cmd)) {
    if (ret == 0) {
      DEBUG_PRINT(F("Failed, retrying attempt: "));
    }
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(ret + 1);
    DEBUG_PRINT(F("/5)..."));

    // Broadcast Delay
    delay(LORA_BROADCAST_DELAY);
    ret++;
  }
  if (ret == 5) {
    DEBUG_PRINTLN(F("Failed"));
  } else {
    DEBUG_PRINTLN(F("Success"));
  }
  DEBUG_PRINTLINE();
}

static int at_send_check_response(bool printResponse, String p_ack_str, unsigned long timeout_ms, String p_cmd_str, ...) {
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
      delay(5);
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
  DEBUG_PRINTLINE();
  return true;
}

// Append package to cmd string
void LoRaBuildCommand(uint8_t package[40], char cmd[128], int size) {
  char *cmdPtr = cmd;
  cmdPtr += sprintf(cmdPtr, "AT+CMSGHEX=\"");

  for (int i = 0; i < size; i++) {
    if (i > 0) {
      cmdPtr += sprintf(cmdPtr, " ");
    }
    cmdPtr += sprintf(cmdPtr, "%02X", package[i]);
  }
  cmdPtr += sprintf(cmdPtr, "\"\r\n");
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

  uint8_t package[45];
  int size = 0;

  // Read line from log file
  bool endOfPackage = LogReadLine(package, size);

  // Build Lora package
  char cmd[128];
  LoRaBuildCommand(package, cmd, size);

  // Send package over LoRa
  int ret = 0;
  while (ret < 5 && !at_send_check_response(false, "Done", 5000, cmd)) {
    ret++;
  }

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

  uint8_t package[40];
  int size = 0;

  // Read line from Data file
  bool endOfPackage = DataReadLine(package, size);

  // Build Lora package
  char cmd[129];
  LoRaBuildCommand(package, cmd, size);

  DEBUG_PRINT(F("Sending package ("));
  DEBUG_PRINT(lineRead + 1);
  DEBUG_PRINT(F(" of "));
  DEBUG_PRINT(GetDataLines());
  DEBUG_PRINTLN(F(")"));
  // DEBUG_PRINT(cmd);

  // Send package over LoRa
  int ret = 0;
  while (ret < MAX_LORA_BROADCAST && !at_send_check_response(false, "RXWIN1", 10000, cmd) && size > 0) {
    if (ret == 0) {
      DEBUG_PRINT(F("Failed, retrying attempt: "));
    }
    DEBUG_PRINT(F("("));
    DEBUG_PRINT(ret + 1);
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT(MAX_LORA_BROADCAST);
    DEBUG_PRINT(F(")... "));

    // Broadcast Delay
    delay(LORA_BROADCAST_DELAY);

    ret++;
  }
  // DEBUG_PRINTLN(F("Success"));
  // DEBUG_PRINTLINE();

  lineRead++;

  // Limit broadcast attemps
  if (ret == MAX_LORA_BROADCAST) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("LoRa Broadcast failed"));
    DEBUG_PRINTLINE();
    return true;
  }

  return endOfPackage;
}
