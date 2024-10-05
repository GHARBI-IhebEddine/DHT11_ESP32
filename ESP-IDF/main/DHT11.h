#include <stdint.h>
#include <math.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif
class DHT11{
public:
    DHT11(gpio_num_t pin);
    void begin();
    bool read();
    float readTemp();
    float readHum();
    uint16_t read_pulse_length(gpio_num_t pin, bool state, uint16_t timeout);
private:
    gpio_num_t myPin;
    uint16_t data[3]; 
    uint64_t lastRead;
};

#ifdef __cplusplus
}
#endif
