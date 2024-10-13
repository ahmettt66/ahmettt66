#include "MS5837.h"

MS5837 sensor;

int temp=0;

struct Press5837 {
  float pressure;
  float temp;
  float depth;
  bool Error;
};

float Prevpressure, Prevtemp, Prevdepth = 0.0;

struct Press5837 PrevPress5837;

struct Press5837 GetPress5837Values() {
  struct Press5837 ms;
  ms.pressure = Prevpressure;
  ms.temp = Prevtemp;
  ms.depth = Prevdepth;
  ms.Error = false;

  sensor.read();



  ms.pressure = sensor.pressure();

  ms.temp = sensor.temperature();

  ms.depth = sensor.depth();

  Prevpressure =  ms.pressure;
  Prevtemp =  ms.temp;
  Prevdepth = ms.depth;

  if(sensor.init()==0){
    ms.Error=true;
  }

  return ms;
}

void initializeMS5837() {
  sensor.init();
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997);
}
