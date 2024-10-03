#include "DHT11.h"
#define pin 21

DHT11 dht(pin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //dht.begin();
  delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(dht.readTemp());
}
