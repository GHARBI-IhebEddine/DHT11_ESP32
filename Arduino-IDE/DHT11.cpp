#include "esp32-hal.h"
#include "DHT11.h"

DHT11::DHT11(uint8_t pin) {
  myPin = pin;
  pinMode(myPin, INPUT_PULLUP);
  lastRead = millis();
}

bool DHT11::read(){
  if(millis() - lastRead < 200) return true;
  lastRead = millis();
  data[0] = data[1] = data[2] = 0;
  pinMode(myPin, OUTPUT);
  digitalWrite(myPin, LOW);
  delay(18);
  pinMode(myPin, INPUT_PULLUP);
  noInterrupts();
  uint16_t pulse = pulseIn(myPin, HIGH, 180);
  if(pulse < 75) return false;
  for(int i = 0; i<40; i++){
    uint16_t value = pulseIn(myPin, HIGH, 150);
    if (i < 16) {
      data[0] = (data[0] << 1) | (value > 50);
    } else if (i < 32) {
      data[1] = (data[1] << 1) | (value > 50);
    } else {
      data[2] = (data[2] << 1) | (value > 50);
    }
  }
  interrupts();
  if(data[2] != ((data[1] & 0xFF) + (data[0] & 0xFF) + ((data[1] >> 8) & 0xFF) + ((data[0] >> 8) & 0xFF)))
    return false;
  return true;
}

float DHT11::readTemp(){
  if(read()){
    return data[1]*0.1;
  }
  return NAN;
}

float DHT11::readHum(){
  if(read()){
    return data[0]*0.1;
  }
  return NAN;
}