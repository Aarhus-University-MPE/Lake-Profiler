
#include "../setup/modules.h"

// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  Wire.begin();
  Wire.setWireTimeout(1000);

  pinMode(LED_BUILTIN, OUTPUT);
}
