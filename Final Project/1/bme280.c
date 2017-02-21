/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    bme280.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#include "bme280.h"

/*----------------------------------------------------------------------------
  Calibration values.
 *---------------------------------------------------------------------------*/
volatile uint16_t dig_T1 = 0;
volatile int16_t dig_T2 = 0;
volatile int16_t dig_T3 = 0;
volatile uint16_t dig_P1 = 0;
volatile int16_t dig_P2 = 0;
volatile int16_t dig_P3 = 0;
volatile int16_t dig_P4 = 0;
volatile int16_t dig_P5 = 0;
volatile int16_t dig_P6 = 0;
volatile int16_t dig_P7 = 0;
volatile int16_t dig_P8 = 0;
volatile int16_t dig_P9 = 0;
volatile uint16_t dig_H1 = 0;
volatile uint16_t dig_H3 = 0;
volatile int16_t dig_H2 = 0;
volatile int16_t dig_H4 = 0;
volatile int16_t dig_H5 = 0;
volatile int16_t dig_H6 = 0;
volatile int32_t t_fine = 0;

/*----------------------------------------------------------------------------
  Normal sensor moder.
 *---------------------------------------------------------------------------*/ 
void normal_sensor_mode(void) {
	//ENTER TO SLEEP MODE
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(0x00);																																	//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    		 	
	//HUMADITY CONFIGURATION
  enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_HUM_CTRL);																												//send register adress
	write_byte_i2c0(BME280_HUM_OVERSAMPLING);																								//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    
	//CONFIG REGISTER
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_CONFIG);																													//send register adress
	write_byte_i2c0(BME280_STANDBY_FILTER);																									//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
	//PRESSURE CONFIGURATION
  enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(BME280_MODE);																														//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
}

/*----------------------------------------------------------------------------
  Gamin sensor mode.
 *---------------------------------------------------------------------------*/ 
void gaming_sensor_mode(void) {
	//ENTER TO SLEEP MODE
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(0x00);																																	//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    		 	
	//HUMADITY CONFIGURATION
  enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_HUM_CTRL);																												//send register adress
	write_byte_i2c0(BME280_HUM_OVERSAMPLING0);																								//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    
	//CONFIG REGISTER
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_CONFIG);																													//send register adress
	write_byte_i2c0(BME280_STANDBY_FILTER_GAMING);																									//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
	//PRESSURECONFIGURATION
  enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(BME280_MODE_GAMING);																														//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
}


/*----------------------------------------------------------------------------
  Function depend for get calibration registers from BME280 sensor and 
	calibrate sensor.
 *---------------------------------------------------------------------------*/ 
void initialize_bme280(void) {
	volatile uint16_t tmp_1 = 0;
	volatile uint16_t tmp_2 = 0;
	volatile uint16_t tmp_3 = 0;	
	t_fine = 0;	
	//READ dig_T1
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T1_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T1_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_T1 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_T2
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T2_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T2_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_T2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_T3
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T3_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_T3_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_T3 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P1
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P1_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P1_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P1 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P2
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P2_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P2_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P3
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P3_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P3_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P3 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P4
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P4_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P4_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P4 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P5
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P5_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P5_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P5 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P6
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P6_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P6_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P6 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P7
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P7_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P7_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P7 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P8
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P8_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P8_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P8 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P9
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P9_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_P9_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_P9 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_H1
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H1);																													//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module
	dig_H1 = tmp_1;																																					//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation	
	//READ dig_H2
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H2_LSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module	
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H2_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module																																																	
	dig_H2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_H3
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H3);																													//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress		
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module
	dig_H3 = tmp_1;																																					//assign proper value to proper registers
	tmp_1 = 0;																																							//reset value before next operation
	//READ dig_H4 and dig_H5
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H4_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H4_LSB_H5_LSB);																							//send register adress
	restart_i2c0();    																																			//restart transmission	
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module
  enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H5_MSB);																											//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_3 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0(); 																																				//disable module
	dig_H4 = (tmp_1 << 4) | (tmp_2 & 0x0f);																									//assign proper value to proper registers	
  dig_H5 = (tmp_3 << 4) | ((tmp_2>>4) & 0x0f);																						//assign proper value to proper registers
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	tmp_3 = 0;																																							//reset value before next operation
	//READ dig_H6
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_DIG_H6);																													//send register adress
	restart_i2c0();    																																			//restart transmission
	write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
	tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();   																																			//disable module
	dig_H6 = tmp_1;																																					//assign proper value to proper registers
	tmp_1 = 0; 																																							//reset value before next operation	
	//ENTER TO SLEEP MODE
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(0x00);																																	//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    		 	
	//HUMADITY CONFIGURATION
  enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_HUM_CTRL);																												//send register adress
	write_byte_i2c0(BME280_HUM_OVERSAMPLING);																								//send value
	stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();    
	//CONFIG REGISTER
	enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_CONFIG);																													//send register adress
	write_byte_i2c0(BME280_STANDBY_FILTER);																									//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
	//HUMADITY CONFIGURATION
  enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit	
	write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c0(BME280_MEAS_CTRL);																											//send register adress
	write_byte_i2c0(BME280_MODE);																														//send value
	stop_i2c0();																																						//stop i2c0 transmmsion	
	disable_i2c0();																																					//disable i2c0
}

/*----------------------------------------------------------------------------
  Function depend for reading temperature from BME280.
 *---------------------------------------------------------------------------*/
float get_bme280_temperature(void) {		
	  volatile int32_t temp = 0;
    volatile float tempf = 0;
		volatile uint16_t tmp_1 = 0;
		volatile uint16_t tmp_2 = 0;
		volatile uint16_t tmp_3 = 0;
    volatile uint32_t temp_raw = 0;		
 		enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_TEMP_MSB);																												//send register adress
		restart_i2c0();                           																							//send start bit
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
		tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();     																																				//stop transmission, send stop bit
		disable_i2c0();																																					//disable module
		enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_TEMP_LSB);																												//send register adress
		restart_i2c0();    																																			//restart transmission	
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
		tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0(); 																																				//disable module			
		enable_i2c0();                           																								//enable i2c0
 		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_TEMP_XLSB);																											//send register adress
		restart_i2c0();    																																			//restart transmission	
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
		tmp_3 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0();																																					//disable module
    temp_raw = (tmp_1 << 12) | (tmp_2 << 4) | (tmp_3 >> 4);
    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14); 
    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp; 
    return (tempf/100.0f);	
}

/*----------------------------------------------------------------------------
  Function depend for reading humadity from BME280.
 *---------------------------------------------------------------------------*/
float get_bme280_humadity(void) {
		volatile uint16_t tmp_1 = 0;
		volatile uint16_t tmp_2 = 0;
		volatile uint32_t hum_raw = 0;	
    volatile int32_t v_x1 = 0;
    volatile float humf;
    enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_HUM_MSB);																												//send register adress
		restart_i2c0();    																																			//restart transmission
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
		tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0();   																																			//disable module	
		enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_HUM_LSB);																												//send register adress
		restart_i2c0();    																																			//restart transmission	
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
		tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0(); 																																				//disable module	 
    hum_raw = (tmp_1<< 8) | tmp_2;
    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1); 
    humf = (float)(v_x1 >> 12); 
    return (humf/1024.0f);
}

/*----------------------------------------------------------------------------
  Function depend for reading pressure from BME280.
 *---------------------------------------------------------------------------*/
float get_bme280_pressure(void) {
    volatile uint32_t press_raw = 0;
    volatile float pressf = 0;
		volatile uint16_t tmp_1 = 0;
		volatile uint16_t tmp_2 = 0;
		volatile uint16_t tmp_3 = 0; 
    volatile int32_t var1 = 0;
		volatile int32_t var2 = 0;
    volatile uint32_t press = 0;	
		enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_PRESS_MSB);																											//send register adress
		restart_i2c0();    																																			//restart transmission
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress
		tmp_1 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0();   																																			//disable module	
		enable_i2c0();                           																								//enable i2c0
		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_PRESS_LSB);																											//send register adress
		restart_i2c0();    																																			//restart transmission	
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
		tmp_2 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0(); 																																				//disable module
		enable_i2c0();                           																								//enable i2c0		
 		start_i2c0();                           																								//send start bit	
		write_byte_i2c0(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c0(BME280_PRESS_XLSB);																											//send register adress
		restart_i2c0();    																																			//restart transmission	
		write_byte_i2c0(BME280_READ_ADRESS);																										//send read_adress	
		tmp_3 = read_byte_i2c0(I2C0_NACK); 																											//read value
		stop_i2c0();                             																								//stop transmission, send stop bit
		disable_i2c0(); 					
    press_raw = (tmp_1 << 12) | (tmp_2 << 4) | (tmp_3 >> 4);
    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 1);
    var2 = (var2 >> 2) + (dig_P4 << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * dig_P1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
    }
    var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
    press = (press + ((var1 + var2 + dig_P7) >> 4)); 
    pressf = (float)press;
    return (pressf/100.0f);
}
