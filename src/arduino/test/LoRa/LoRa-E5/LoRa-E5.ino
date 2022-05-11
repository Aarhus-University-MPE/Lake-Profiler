/*
  LoRa-E5

  Code:
    https://github.com/sufiankaki/lora-e5/blob/main/ESP32%20Dev%20Module/SendReceiveJson-TTN/SendReceiveJson-TTN.ino

  Module:
    https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/
*/

#define e5 Serial1

static char recv_buf[512];
static bool is_exist = false;
static bool is_join  = false;
static int led       = LED_BUILTIN;
int counter          = 0;

// Transmit function
static int at_send_check_response(const char *p_ack, int timeout_ms, const char *p_cmd, ...) {
  int ch;
  int num                   = 0;
  int index                 = 0;
  unsigned long startMillis = 0;

  va_list args;
  memset(recv_buf, 0, sizeof(recv_buf));
  va_start(args, p_cmd);
  e5.print(p_cmd);
  Serial.print(p_cmd);
  va_end(args);
  delay(200);
  startMillis = millis();

  if (p_ack == NULL)
    return 0;

  do {
    while (e5.available() > 0) {
      ch                = e5.read();
      recv_buf[index++] = ch;
      Serial.print((char)ch);
      delay(2);
    }

    if (strstr(recv_buf, p_ack) != NULL)
      return 1;

  } while (millis() - startMillis < timeout_ms);
  Serial.println();
  return 0;
}

// Receive function
static void recv_prase(char *p_msg) {
  if (p_msg == NULL) {
    Serial.println("Received null");
    return;
  }
  char *p_start = NULL;
  char data[128];  // To hold the received bytes as characters

  int bytes_len = 0;
  p_start       = strstr(p_msg, "TX");
  // Serial.println(p_start);
  if (p_start && (1 == sscanf(p_start, "+TEST: TX %s", &data))) {
    // Serial.println(data);
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  e5.begin(9600);
  Serial.print("E5 LOCAL TEST\r\n");

  if (at_send_check_response("+AT: OK", 100, "AT\r\n")) {
    is_exist = true;
    at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
    at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
    at_send_check_response("+DR: EU868", 1000, "AT+DR=EU868\r\n");
    at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,0-2\r\n");
    at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"8DBC89820C1559CC1336B5395873FD15\"\r\n");
    at_send_check_response("+CLASS: C", 1000, "AT+CLASS=A\r\n");
    at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
    delay(200);
    is_join = true;
    digitalWrite(led, HIGH);
  } else {
    is_exist = false;
    Serial.print("No E5 module found.\r\n");
  }
}

void loop(void) {
  if (is_exist) {
    int ret = 0;
    if (is_join) {
      ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
      if (ret)
        is_join = false;
      else {
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        Serial.print("JOIN failed!\r\n\r\n");
        delay(5000);
      }
    } else {
      // StaticJsonDocument<200> json;
      // json["state"]["temp"] = 27.4;
      // json["state"]["humi"] = 89;

      // char charArray[64];
      // int len                             = serializeJson(json, charArray);
      // char buildBuffer[2]                 = {0};
      // char compositionBuffer[len * 3 + 1] = {0};  // this will hold a string we build

      // for (int i = 0; i < len; i++) {
      //   sprintf(buildBuffer, "%02X ", (uint8_t)charArray[i]);
      //   strcat(compositionBuffer, buildBuffer);
      // }

      // char cmd[512];
      // sprintf(cmd, "AT+CMSGHEX=\"");
      // strcat(cmd, compositionBuffer);
      // strcat(cmd, "\"\r\n");
      // ret = at_send_check_response("Done", 5000, cmd);

      // if (ret)
      //   recv_prase(recv_buf);
      // else
      //   Serial.print("Send failed!\r\n\r\n");
      // delay(5000);
    }
  } else
    delay(1000);
}

// void loop(void) {
//   char cmd[128];
//   counter = counter + 1;
//   // Transmit HEX Value
//   sprintf(cmd, "AT+TEST=TXLRPKT,\"%d\"\r\n", counter);
//   int ret = at_send_check_response("+TEST: TXLRPKT", 5000, cmd);

//   // Transmit string
//   // sprintf(cmd, "AT+TEST=TXLRSTR,\"%d\"\r\n", counter);
//   // ret = at_send_check_response("+TEST: TXLRSTR", 5000, cmd);

//   if (ret)
//     Serial.println("Sent");
//   else
//     Serial.println("Send failed!\r\n\r\n");
//   delay(5000);
// }