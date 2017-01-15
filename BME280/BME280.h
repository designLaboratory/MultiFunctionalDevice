#ifndef BME280_H
#define BME280_H

#include "MKL46Z4.h"

void initialize_bme280(void);
void initialize_i2c(void);
void disable_i2c(void);
uint8_t write_byte_i2c(uint8_t data);
void stop_i2c(void);
uint8_t read_byte_i2c(uint8_t ack);
void restart_i2c(void);
void start_i2c(void);
void enable_i2c(void);
float get_bme280_humadity(void);
float get_bme280_pressure(void);
float get_bme280_temperature(void);
#endif
