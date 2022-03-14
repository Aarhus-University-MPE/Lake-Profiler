#include "constants.h"
#include "pinout.h"

bool SystemStatus[MODULE_COUNT];

// bool GetStatus(int module);
// void SetStatus(int module, bool status);

bool GetStatus(int module)
{
  return SystemStatus[module];
}
void SetStatus(int module, bool status)
{
  SystemStatus[module] = status;
}

unsigned long ToLong(bool b[]) {
  unsigned long c = 0;
  for (int i = 0; i < MODULE_COUNT; ++i) {
    if (b[i])
      c |= 1L << i;
  }
  return c;
}