#include "DHT11.h"
#include <rom/ets_sys.h>

// pulse length function
extern "C" uint16_t DHT11::read_pulse_length(gpio_num_t pin, bool state, uint16_t timeout) {
	uint16_t loops = 0, width = 0;
	while(gpio_get_level(pin) == state){ 
		if(loops++ >= timeout) 
			return 0;
	}
    while(gpio_get_level(pin) != state) {
    	if(loops++ >= timeout) 
    		return 0;
    }
    while(gpio_get_level(pin) == state){
		width++;
		if(loops++ >= timeout) 
    		return 0;
    }
	return width;
}

extern "C" DHT11::DHT11(gpio_num_t pin) {
    myPin = pin;
}
extern "C" void DHT11::begin(){
	data[0] = data[1] = 0x05; 
	data[2] = 0x01;
	lastRead = esp_timer_get_time();
	gpio_set_direction(myPin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_pull_mode(myPin, GPIO_PULLUP_ONLY);
}

extern "C" bool DHT11::read() {
    uint64_t currentTime = esp_timer_get_time();
    if ((currentTime - lastRead) < 300000) { //300 ms delay
        if (data[2] != ((data[1] & 0xFF) + (data[0] & 0xFF) + ((data[1] >> 8) & 0xFF) + ((data[0] >> 8) & 0xFF))) {
        	return false;
    	}
        return true;
    }
    lastRead = currentTime;
    data[0] = data[1] = data[2] = 0;
    
    // Start communication with DHT11
    gpio_set_direction(myPin, GPIO_MODE_OUTPUT);
    gpio_set_level(myPin, 0);  // Set pin LOW
    vTaskDelay(18 / portTICK_PERIOD_MS);  // Wait for 18ms
    gpio_set_direction(myPin, GPIO_MODE_INPUT);  // Set pin as input
    gpio_set_pull_mode(myPin, GPIO_PULLUP_ONLY);

    // Measure pulse duration to detect the start of transmission
    uint16_t pulse = read_pulse_length(myPin, 1, 1500);
    if (pulse < 150) return false;

    // Read the 40 bits of data from the DHT11 sensor
    for (int i = 0; i < 40; i++) {
        pulse = read_pulse_length(myPin, 1, 500);
        if (i < 16) {
            data[0] = (data[0] << 1) | (pulse > 150);
        } else if (i < 32) {
            data[1] = (data[1] << 1) | (pulse > 150);
        } else {
            data[2] = (data[2] << 1) | (pulse > 150);
        }
    }
    // Validate checksum
    if (data[2] != ((data[1] & 0xFF) + (data[0] & 0xFF) + ((data[1] >> 8) & 0xFF) + ((data[0] >> 8) & 0xFF))) {
        return false;
    }
    return true;
}

extern "C" float DHT11::readTemp() {
    if (read()) {
		return (float)data[1]*0.1;
    }
    return NAN;
}

extern "C" float DHT11::readHum() {
    if (read()) {
        return (float)data[0]*0.1;  // Humidity data in percentage
    }
    return NAN;
}
