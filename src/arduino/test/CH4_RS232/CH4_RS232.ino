/*
  Lake Profiler Canister

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

//const String testCMD = "WTF\r\n";
const String testCMD = "CODPV,0,0,R\r\n";
bool msgSent         = false;

void setup() {
  Serial.begin(115200);
  Serial.print(F("Initializing RS232... "));

  if (RS232Initialize()) {
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

unsigned long lastMillisSent;
char data[10];
void loop() {
  // if (millis() - lastMillisSent > 1000) {
  //   lastMillisSent = millis();
  //   Serial1.print(testCMD);
  //   msgSent = true;
  //   Serial.println();
  //   Serial.println("--------");
  //   Serial.println("Sent msg");
  // }
  if(Serial1.available()>0) {
    Serial.write(Serial1.read());
  }
}

bool RS232Initialize() {
  Serial1.begin(9600);
  return Serial1;
}

bool CH4Initialize() {
  return true;
}