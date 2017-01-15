#include "BME280.h"

#define BME280_WRITE_ADRESS	0xEC																													//BME280 write adress
#define BME280_READ_ADRESS 0xED																												    //BME280 read adress
#define BME280_HUM_CTRL 0xF2																															//BME280 humadity control register
#define BME280_HUM_OVERSAMPLING 0x01																											//humadity oversampling x1
#define BME280_MEAS_CTRL 0xF4																															//measure ctrl register
#define BME280_MODE 0x27																																	//normal moder, oversampling temperature x1, oversampling pressure x1
#define BME280_CONFIG 0xF5																																//config register
#define BME280_STANDBY_FILTER 0xA0  																											//standby 500ms, Filter off
#define BME280_RESET 0xE0
#define BME280_RESET_VALUE 0xB6

//calibrate registers adress
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

//registers which temperature, pressure and humadity
#define BME280_HUM_LSB 0xFE
#define BME280_HUM_MSB 0xFD
#define BME280_TEMP_XLSB 0xFC
#define BME280_TEMP_LSB 0xFB
#define BME280_TEMP_MSB 0xFA
#define BME280_PRESS_XLSB 0xF9
#define BME280_PRESS_LSB 0xF8
#define BME280_PRESS_MSB 0xF7

#define I2C_ACK 0																																					//predefiniton of ACK - 0
#define I2C_NACK 1																																				//predefiniton of NACK - 1

//calibration  registers
volatile uint16_t    dig_T1 = 0;
volatile int16_t     dig_T2 = 0;
volatile int16_t			dig_T3 = 0;
volatile uint16_t    dig_P1 = 0;
volatile int16_t     dig_P2 = 0;
volatile int16_t 		dig_P3 = 0;
volatile int16_t 		dig_P4 = 0;
volatile int16_t 		dig_P5 = 0;
volatile int16_t 		dig_P6 = 0;
volatile int16_t 		dig_P7 = 0;
volatile int16_t 		dig_P8 = 0;
volatile int16_t 		dig_P9 = 0;
volatile uint16_t    dig_H1 = 0;
volatile uint16_t  	dig_H3 = 0;
volatile int16_t     dig_H2 = 0;
volatile int16_t     dig_H4 = 0;
volatile int16_t     dig_H5 = 0;
volatile int16_t     dig_H6 = 0;
volatile int32_t     t_fine = 0;


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
 		enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_TEMP_MSB);																												//send register adress
		restart_i2c();                           																								//send start bit
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
		tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();     																																				//stop transmission, send stop bit
		disable_i2c();																																					//disable module
		enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_TEMP_LSB);																												//send register adress
		restart_i2c();    																																			//restart transmission	
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
		tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c(); 																																					//disable module			
		enable_i2c();                           																								//enable i2c0
 		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_TEMP_XLSB);																												//send register adress
		restart_i2c();    																																			//restart transmission	
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
		tmp_3 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c();																																					//disable module
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
    enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_HUM_MSB);																													//send register adress
		restart_i2c();    																																			//restart transmission
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
		tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c();   																																				//disable module	
		enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_HUM_LSB);																													//send register adress
		restart_i2c();    																																			//restart transmission	
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
		tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c(); 																																					//disable module	 
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
		enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_PRESS_MSB);																												//send register adress
		restart_i2c();    																																			//restart transmission
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
		tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c();   																																				//disable module	
		enable_i2c();                           																								//enable i2c0
		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_PRESS_LSB);																												//send register adress
		restart_i2c();    																																			//restart transmission	
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
		tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c(); 																																					//disable module
		enable_i2c();                           																								//enable i2c0		
 		start_i2c();                           																									//send start bit	
		write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
		write_byte_i2c(BME280_PRESS_XLSB);																											//send register adress
		restart_i2c();    																																			//restart transmission	
		write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
		tmp_3 = read_byte_i2c(I2C_NACK); 																												//read value
		stop_i2c();                             																								//stop transmission, send stop bit
		disable_i2c(); 					
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
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T1_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T1_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_T1 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_T2
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T2_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T2_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_T2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_T3
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T3_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_T3_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_T3 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P1
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P1_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P1_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P1 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P2
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P2_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P2_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P3
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P3_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P3_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P3 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P4
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P4_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P4_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P4 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P5
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P5_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P5_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P5 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P6
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P6_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P6_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P6 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P7
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P7_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P7_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P7 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_P8
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P8_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P8_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P8 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation	
	//READ dig_P9
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P9_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_P9_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_P9 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_H1
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H1);																													//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module
	dig_H1 = tmp_1;																																					//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation	
	//READ dig_H2
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H2_LSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module	
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H2_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress	
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module																																																	
	dig_H2 = (tmp_2 << 8) | tmp_1;																													//assign proper value to proper registers	
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	//READ dig_H3
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H3);																													//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress		
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module
	dig_H3 = tmp_1;																																					//assign proper value to proper registers
	tmp_1 = 0;																																							//reset value before next operation
	//READ dig_H4 and dig_H5
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H4_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H4_LSB_H5_LSB);																								//send register adress
	restart_i2c();    																																			//restart transmission	
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_2 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module
  enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H5_MSB);																											//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_3 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c(); 																																					//disable module
	dig_H4 = (tmp_1 << 4) | (tmp_2 & 0x0f);																									//assign proper value to proper registers	
  dig_H5 = (tmp_3 << 4) | ((tmp_2>>4) & 0x0f);																						//assign proper value to proper registers
	tmp_1 = 0;																																							//reset value before next operation
	tmp_2 = 0;																																							//reset value before next operation
	tmp_3 = 0;																																							//reset value before next operation
	//READ dig_H6
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_DIG_H6);																													//send register adress
	restart_i2c();    																																			//restart transmission
	write_byte_i2c(BME280_READ_ADRESS);																											//send read_adress
	tmp_1 = read_byte_i2c(I2C_NACK); 																												//read value
  stop_i2c();                             																								//stop transmission, send stop bit
  disable_i2c();   																																				//disable module
	dig_H6 = tmp_1;																																					//assign proper value to proper registers
	tmp_1 = 0; 																																							//reset value before next operation	
	//ENTER TO SLEEP MODE
	enable_i2c();                           																								//enable i2c0 module
  start_i2c();                            																								//start transmission and send start bit
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_MEAS_CTRL);																												//send register adress
	write_byte_i2c(0x00);																																		//send value
	stop_i2c();                             																								//stop i2c0 transmmsion
  disable_i2c();    		 	
	//HUMADITY CONFIGURATION
  enable_i2c();                           																								//enable i2c0 module
  start_i2c();                            																								//start transmission and send start bit
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_HUM_CTRL);																												//send register adress
	write_byte_i2c(BME280_HUM_OVERSAMPLING);																								//send value
	stop_i2c();                             																								//stop i2c0 transmmsion
  disable_i2c();    
	//CONFIG REGISTER
	enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_CONFIG);																													//send register adress
	write_byte_i2c(BME280_STANDBY_FILTER);																									//send value
	stop_i2c();																																							//stop i2c0 transmmsion	
	disable_i2c();																																					//disable i2c0
	//HUMADITY CONFIGURATION
  enable_i2c();                           																								//enable i2c0
  start_i2c();                           																									//send start bit	
	write_byte_i2c(BME280_WRITE_ADRESS);  																									//send write_adress
	write_byte_i2c(BME280_MEAS_CTRL);																												//send register adress
	write_byte_i2c(BME280_MODE);																														//send value
	stop_i2c();																																							//stop i2c0 transmmsion	
	disable_i2c();																																					//disable i2c0
																																													//disable i2c0
}



/*----------------------------------------------------------------------------
  Function depend for enable i2c module.
 *---------------------------------------------------------------------------*/
void enable_i2c(void) {	  
	  I2C0->C1 |= I2C_C1_IICEN_MASK;																												//enable i2c0 module, bit IICEN set to 1
}

/*----------------------------------------------------------------------------
  Function depend for start i2c transmission.
 *---------------------------------------------------------------------------*/
void start_i2c(void) {  
  I2C0->C1 |= I2C_C1_MST_MASK;																														//choose master option and send start bit
}

/*----------------------------------------------------------------------------
  Function depend for restart i2c transmission and generate restart bit.
 *---------------------------------------------------------------------------*/
void restart_i2c(void) {
  I2C0->C1 |= I2C_C1_RSTA_MASK;																														//generate restart bit, restart transmission
}

/*----------------------------------------------------------------------------
  Function depend for read bytes from BME280 sensor.
 *---------------------------------------------------------------------------*/
uint8_t read_byte_i2c(uint8_t ack) {
  I2C0->C1 &= ~I2C_C1_TX_MASK;																														//device will be in receive mode																			
  if((I2C0->SMB & I2C_SMB_FACK_MASK) == 0) 																								//check FACK bit, if is no set then
		I2C0->C1 = (ack == I2C_NACK) ? 																												//if nack is recived
			I2C0->C1 | I2C_C1_TXAK_MASK:																												//send ack
			I2C0->C1 & ~I2C_C1_TXAK_MASK;																												//if not not send ack 
  I2C0->S |= I2C_S_IICIF_MASK;																														//clear IICIF flag, interrupt flag 
  I2C0->D;																																								//receive and discard wrong values
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);																								//check if end of transmission and ack is received 
  if((I2C0->SMB & I2C_SMB_FACK_MASK) != 0)																								//if FACK is 1
    I2C0->C1 = (ack == I2C_NACK) ? 																												//if nack is received
				I2C0->C1 | I2C_C1_TXAK_MASK: 																											//send ack
				I2C0->C1 & ~I2C_C1_TXAK_MASK;																											//send nack
  I2C0->C1 |= I2C_C1_TX_MASK;																															//transmit mode 
  return I2C0->D;																																					//reeive proper value
}
 
/*----------------------------------------------------------------------------
  Function depend for stop i2c transmission.
 *---------------------------------------------------------------------------*/ 
void stop_i2c(void) {
  I2C0->FLT |= I2C_FLT_STOPF_MASK;																												//clear stop flag
  I2C0->C1 &= ~I2C_C1_MST_MASK;																														//set slave option and generate stop bit
  while((I2C0->FLT & I2C_FLT_STOPF_MASK) == 0){																						//wait for stop bit
    I2C0->C1 &= ~I2C_C1_MST_MASK;																													//set slave option and generate stop bit
  }
}

/*----------------------------------------------------------------------------
  Function depend for write data to BME280 registers.
 *---------------------------------------------------------------------------*/  
uint8_t write_byte_i2c(uint8_t data) {
  I2C0->C1 |= I2C_C1_TX_MASK;																															//transmit mode
  I2C0->S |= I2C_S_IICIF_MASK;																														//clear interrupt flag 
  I2C0->D = data;																																					//write data			
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);																								//wait for end of transsmison and get ack
  return ((I2C0->S & I2C_S_RXAK_MASK) == I2C_S_RXAK_MASK ? I2C_NACK : I2C_ACK);						//return ack or nack
}

/*----------------------------------------------------------------------------
  Function depend for disable i2c module.
 *---------------------------------------------------------------------------*/ 
void disable_i2c(void) {
  I2C0->C1 &= ~I2C_C1_IICEN_MASK;																													//iicen to 0, disable i2c module
}

/*----------------------------------------------------------------------------
  Function depend for initialize i2c peripherials and get calibration registers.
 *---------------------------------------------------------------------------*/ 
void initialize_i2c(void) {
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;																											//clock for i2c module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;																											//clock for port e where are SCL and SDA
	PORTE->PCR[18] |= PORT_PCR_MUX(4);																											//PORTE18 is SDA
	PORTE->PCR[19] |= PORT_PCR_MUX(4);																											//PORTE19 is CLK
	I2C0->F   = I2C_F_MULT(2) | I2C_F_ICR(0x2D);                    												//set baudrate: 9600
	initialize_bme280();
}
