/*
  Lake Profiler System Control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include <Arduino.h>

unsigned long lastSystemCheck = 9999999;

void SystemEnable(int module);

void SystemDisable(int module);

// Disable all secondary systems
void SystemDisable();
// Run full system test
bool SystemTest();
bool SystemTestModule(byte module);
// Checks all Systems
void SystemCheck();
bool SystemCheckModule(byte module);