//Güç dağıtım kartı eklenecek.
#include <Base.h>
Base base;

int voltage, current, power = 0;

struct BaseModule {
  float voltage;
  float current;
  float power;
  int capacity;
  bool Error;
};

float Prevvoltage, Prevcurrent, Prevpower = 0.0;
int Prevcapacity = 0;
struct BaseModule PrevBaseModule;

struct BaseModule GetBaseModuleValues() {
  struct BaseModule bm;
  bm.voltage = Prevvoltage;
  bm.current = Prevcurrent;
  bm.power = Prevpower;
  bm.capacity = Prevcapacity;
  bm.Error = false;

  bm.voltage = base.busVoltage();
  bm.current = base.shuntCurrent();
  bm.power = base.busPower();

  Prevvoltage =  bm.voltage;
  Prevcurrent =  bm.current;
  Prevpower = bm.power;

  if (bm.voltage <= 25.2) {
    bm.capacity = 100;
  }
  else if (bm.voltage <= 24.9) {
    bm.capacity = 95;
  }
  else if (bm.voltage <= 24.67) {
    bm.capacity = 90;
  }
  else if (bm.voltage <= 24.49) {
    bm.capacity = 85;
  }
  else if (bm.voltage <= 24.14) {
    bm.capacity = 80;
  }
  else if (bm.voltage <= 23.9) {
    bm.capacity = 75;
  }
  else if (bm.voltage <= 23.72) {
    bm.capacity = 70;
  }
  else if (bm.voltage <= 23.48) {
    bm.capacity = 65;
  }
  else if (bm.voltage <= 23.25) {
    bm.capacity = 60;
  }
  else if (bm.voltage <= 23.13) {
    bm.capacity = 55;
  }
  else if (bm.voltage <= 23.01) {
    bm.capacity = 50;
  }
  else if (bm.voltage <= 22.89) {
    bm.capacity = 45;
  }
  else if (bm.voltage <= 22.77) {
    bm.capacity = 40;
  }
  else if (bm.voltage <= 22.72) {
    bm.capacity = 35;
  }
  else if (bm.voltage <= 22.6) {
    bm.capacity = 30;
  }
  else if (bm.voltage <= 22.48) {
    bm.capacity = 25;
  }
  else if (bm.voltage <= 22.36) {
    bm.capacity = 20;
  }
  else if (bm.voltage <= 22.24) {
    bm.capacity = 15;
  }
  else if (bm.voltage <= 22.12) {
    bm.capacity = 10;
  }
  Prevcapacity = bm.capacity;
  return bm;
}


void  initializeBaseModule() {
  base.begin();
  base.currentOffset(-1.48);
}
