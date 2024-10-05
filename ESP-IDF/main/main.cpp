#include <string.h>
#include "DHT11.h"
#include "driver/uart.h"

gpio_num_t PIN = GPIO_NUM_21;

// DHT1 constructor
DHT11 dht(PIN);

// UART initialization function
void uart_init() {
    const uart_port_t uart_num = UART_NUM_0;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(uart_num, &uart_config);
    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);
}

extern "C" void app_main(void) {
    uart_init();  // Initialize UART
	dht.begin(); // Initialize DHT11
	
    while (true) {
		float temp = dht.readTemp(), hum = dht.readHum();
        char buffer[40];
		snprintf(buffer, sizeof(buffer), "Temperature: %.2f °C, Hum: %.2f \n", temp, hum);
        uart_write_bytes(UART_NUM_0, buffer, strlen(buffer));
        
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Delay 2 seconds between readings
    }
}
