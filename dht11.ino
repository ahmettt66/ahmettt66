
#include "DHT.h"

DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

struct DHT11 {
  int h1hum;
  int h1temp;
  int h2hum;
  int h2temp;
  bool Error;
};

int Prevh1hum, Prevh1temp, Prevh2hum, Prevh2temp = 0;


struct DHT11 PrevDHT11;

void initializeDHT() {
  dht1.begin();
  dht2.begin();
}

struct DHT11 GetDHT11Values() {
  struct DHT11 dht;
  dht.h1hum = Prevh1hum;
  dht.h1temp = Prevh1temp;
  dht.h2hum = Prevh2hum;
  dht.h2temp = Prevh2temp;
  dht.Error = false;




  dht.h1hum = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  dht.h1temp = dht1.readTemperature();

  dht.h2hum = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  dht.h2temp = dht2.readTemperature();

  Prevh1hum = dht.h1hum;
  Prevh1temp = dht.h1temp;
  Prevh2hum = dht.h2hum;
  Prevh2temp = dht.h2temp;
  
  if (dht.h1hum==0 || dht.h1temp==0 || dht.h2hum==0 || dht.h2temp==0)  {
    dht.Error = true;
  }
    return dht;
}
