#include <stdint.h>
#include "Arduino.h"

class DHT11 {
public:
  DHT11(uint8_t pin);
  bool read();
  float readTemp();
  float readHum();
private:
  uint8_t myPin;
  uint16_t data[3];
  uint32_t lastRead;
};