/*
  LoRa-E5

  Code:
    https://github.com/sufiankaki/lora-e5/blob/main/ESP32%20Dev%20Module/SendReceiveJson-TTN/SendReceiveJson-TTN.ino

  Module:
    https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/
*/

static char recv_buf[512];
static bool is_exist = false;
static bool is_join  = false;
static int led       = 0;

static int at_send_check_response(String p_ack_str, int timeout_ms, String p_cmd_str, ...) {
  int ch;
  int num           = 0;
  int index         = 0;
  int startMillis   = 0;
  const char *p_ack = p_ack_str.c_str();
  const char *p_cmd = p_cmd_str.c_str();
  char buff[512];

  va_list args;
  memset(recv_buf, 0, sizeof(recv_buf));
  va_start(args, p_cmd_str);
  sprintf(buff, p_cmd, args);
  Serial1.print(buff);
  Serial.print(F("Sending: "));
  Serial.print(buff);
  va_end(args);
  delay(200);
  startMillis = millis();

  if (p_ack == NULL) {
    return 0;
  }

  do {
    while (Serial1.available() > 0) {
      ch                = Serial1.read();
      recv_buf[index++] = ch;
      Serial.print((char)ch);
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
    if (led) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  p_start = strstr(p_msg, "RSSI");
  if (p_start && (1 == sscanf(p_start, "RSSI %d,", &rssi))) {
  }
  p_start = strstr(p_msg, "SNR");
  if (p_start && (1 == sscanf(p_start, "SNR %d", &snr))) {
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial1.begin(9600);
  Serial.print("E5 LORAWAN TEST\r\n");

  while (!is_join) {
    if (at_send_check_response("+AT: OK", 1000, "AT\r\n")) {
      is_exist = true;
      at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
      at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
      at_send_check_response("+DR: EU868", 1000, "AT+DR=EU868\r\n");
      at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,0-2\r\n");
      at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n");
      at_send_check_response("+CLASS: C", 1000, "AT+CLASS=A\r\n");
      at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
      delay(200);
      is_join = true;
    } else {
      is_exist = false;
      Serial.print("No E5 module found.\r\n");
      delay(5000);
      // while (1)
      // ;
    }
  }
}

void loop(void) {
  int ret = 0;
  if (is_join) {
    ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
    if (ret) {
      is_join = false;
    } else {
      at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
      Serial.print("JOIN failed!\r\n\r\n");
      delay(5000);
    }
  } else {
    char cmd[128];
    sprintf(cmd, "AT+CMSGHEX=\"%04X%04X\"\r\n", (int)314, (int)314);
    ret = at_send_check_response("Done", 5000, cmd);
    if (ret) {
      recv_prase(recv_buf);
    } else {
      Serial.print("Send failed!\r\n\r\n");
    }
    delay(5000);
  }
}