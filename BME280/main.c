#include "BME280.h"

int main(void) {		
	volatile float temperature = 0;
	volatile float pressure = 0;
	volatile float humadity = 0;
	initialize_i2c();
	while(1) {		
		temperature = get_bme280_temperature();
		pressure = get_bme280_pressure();
		humadity = get_bme280_humadity();
	}
}

