/*
  Simple Alarm Test Sketch
  Search for next alarm timing
  Pmod RTCC
*/
uint8_t alarmFrequency = 0;
uint8_t alarmStartHour = 0;
uint8_t alarm[24];

byte alarmState;
const byte maxAlarmStates = 24;

void setup() {
  Serial.begin(115200);

  SetAlarmStartHour(6);
  SetAlarmFrequency(4);
  NextAlarm();
}

void loop() {
}

uint8_t NextAlarm() {
  // uint8_t hour = RTCC.getHour(RTCC_RTCC);
  uint8_t hour = 0x19;

  int8_t hourDifference = -1;
  uint8_t i             = 0;

  Serial.print(F("Current hour: "));
  Serial.print(hour, HEX);
  hourDifference = alarm[i] - hour;

  while (hourDifference <= 0 && i < alarmFrequency) {
    i++;
    hourDifference = alarm[i] - hour;
  }
  Serial.print(F(", next closest alarm: "));
  Serial.println(alarm[i], HEX);
}

void SetAlarmFrequency(uint8_t frequency) {
  alarmFrequency = frequency;
  UpdateAlarmTimings();
}

void SetAlarmStartHour(uint8_t startHour) {
  alarmStartHour = startHour;
  UpdateAlarmTimings();
}

void UpdateAlarmTimings() {
  uint8_t alarmIntervals = 24 / alarmFrequency;  // Convert to hex?

  for (uint8_t i = 0; i < alarmFrequency; i++) {
    alarm[i] = HourToHex(alarmStartHour + alarmIntervals * i);
  }
}

uint8_t HourToHex(uint8_t hour) {
  if (hour < 10) {
    return hour;
  } else if (hour < 20) {
    return hour + 6;
  } else if (hour < 24) {
    return hour + 12;
  } else {
    return HourToHex(hour - 24);
  }
}
