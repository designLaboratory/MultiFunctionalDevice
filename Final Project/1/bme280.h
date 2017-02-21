/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    bme280.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#ifndef BME280_H
#define BME280_H

#include "MKL46Z4.h"
#include "i2c0.h"

#define BME280_WRITE_ADRESS	0xEC																													//BME280 write adress
#define BME280_READ_ADRESS 0xED																												    //BME280 read adress
#define BME280_HUM_CTRL 0xF2																															//BME280 humadity control register
#define BME280_HUM_OVERSAMPLING 0x01																											//humadity oversampling x1
#define BME280_HUM_OVERSAMPLING0 0x00																											//humadity oversampling x0 (disabled)
#define BME280_MEAS_CTRL 0xF4																															//measure ctrl register
#define BME280_MODE_GAMING 0x2F																														//normal mode, oversampling temperature x1, oversampling pressyre x6
#define BME280_MODE 0x27																																	//normal moder, oversampling temperature x1, oversampling pressure x1
#define BME280_CONFIG 0xF5																																//config register
#define BME280_STANDBY_FILTER_GAMING 0x10  																											//standby 0.5ms, filter on and coeficients will be 16
#define BME280_STANDBY_FILTER 0xA0																												//standby 500ms, Filter off
#define BME280_RESET 0xE0
#define BME280_RESET_VALUE 0xB6

#define BME280_DIG_T1_LSB 0x88
#define BME280_DIG_T1_MSB 0x89
#define BME280_DIG_T2_LSB 0x8A
#define BME280_DIG_T2_MSB 0x8B
#define BME280_DIG_T3_LSB 0x8C
#define BME280_DIG_T3_MSB 0x8D
#define BME280_DIG_P1_LSB 0x8E
#define BME280_DIG_P1_MSB	0x8F
#define BME280_DIG_P2_LSB 0x90
#define BME280_DIG_P2_MSB 0x91
#define BME280_DIG_P3_LSB 0x92
#define BME280_DIG_P3_MSB 0x93
#define BME280_DIG_P4_LSB 0x94
#define BME280_DIG_P4_MSB 0x95
#define BME280_DIG_P5_LSB 0x96
#define BME280_DIG_P5_MSB 0x97
#define BME280_DIG_P6_LSB 0x98
#define BME280_DIG_P6_MSB 0x99
#define BME280_DIG_P7_LSB 0x9A
#define BME280_DIG_P7_MSB 0x9B
#define BME280_DIG_P8_LSB 0x9C
#define BME280_DIG_P8_MSB 0x9D
#define BME280_DIG_P9_LSB 0x9E
#define BME280_DIG_P9_MSB 0x9F
#define BME280_DIG_H1	0xA1
#define BME280_DIG_H2_LSB	0xE1
#define BME280_DIG_H2_MSB 0xE2
#define BME280_DIG_H3 0xE3
#define BME280_DIG_H4_MSB 0xE4
#define BME280_DIG_H4_LSB_H5_LSB 0xE5
#define BME280_DIG_H5_MSB 0xE6
#define BME280_DIG_H6 0xE7

#define BME280_HUM_LSB 0xFE
#define BME280_HUM_MSB 0xFD
#define BME280_TEMP_XLSB 0xFC
#define BME280_TEMP_LSB 0xFB
#define BME280_TEMP_MSB 0xFA
#define BME280_PRESS_XLSB 0xF9
#define BME280_PRESS_LSB 0xF8
#define BME280_PRESS_MSB 0xF7

void initialize_bme280(void);
float get_bme280_humadity(void);
float get_bme280_pressure(void);
float get_bme280_temperature(void);
void normal_sensor_mode(void);
void gaming_sensor_mode(void);

#endif
