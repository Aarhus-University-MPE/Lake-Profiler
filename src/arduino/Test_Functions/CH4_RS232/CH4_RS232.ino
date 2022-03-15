/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

const String testCMD = "CODPV,0,0,R\r\n";
bool msgSent         = false;

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing RS232... "));
  Serial1.begin(115200);

  if (Serial1) {
    Serial.println(F("Success"));
  } else {
    Serial.println("Error");
    while (1) {
    }
  }

  Serial.print("Initializing CH4 sensor... ");
  if (CH4Initialize) {
    Serial.println("Success");
  } else {
    Serial.println("Error");
    while (1) {
    }
  }
}

void loop() {
  if (!msgSent) {
    Serial2.print(testCMD);
  }
  if (Serial2.available()) {
    Serial.print(Serial2.read());
  }
}

bool CH4Initialize() {
  return true;
}