/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    mag3110.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
#include "mag3110.h"
#include "i2c0.h"

#define MAG3110_ADDR_WRITE 0x1C																														//predefiniton with i2c write adress of MAG3110
#define MAG3110_ADDR_READ 0x1D																														//predefiniton with i2c read adress of MAG3110
#define MAG3110_DIE_TEMP 0x0F																															//predefiniton with i2c temperature register adress in MAG3110
#define MAG3110_CTRL_REG1 0x10																														//predefiniton with i2c control 1 register adress in MAG3110
#define MAG3110_CTRL_REG2 0x11																														//predefiniton with i2c control 2 register adress in MAG3110
#define PI 3.1415																																					//predefiniton with PI constant
#define MAG3110_OUT_X_MSB 0x01																														//predefiniton with i2c out data from x asis msb register adress in MAG3110
#define MAG3110_OUT_X_LSB 0x02																														//predefiniton with i2c out data from x asis lsb register adress in MAG3110
#define MAG3110_OUT_Y_MSB 0x03																														//predefiniton with i2c out data from y asis msb register adress in MAG3110
#define MAG3110_OUT_Y_LSB 0x04	 																													//predefiniton with i2c out data from y asis lsb register adress in MAG3110

/*----------------------------------------------------------------------------
  Function depend for adjust magnetometr, assign values to control 
	registers.
 *---------------------------------------------------------------------------*/
void initialize_mag3110(void) {
  enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start transmission and send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_CTRL_REG2);     																								//write MAG3110 control register 2
  write_byte_i2c0(0x80);     																															//write proper values
  stop_i2c0();                             																								//stop i2c0 transmmsion
  disable_i2c0();                          																								//disable i2c0
  enable_i2c0();                           																								//enable i2c0
  start_i2c0();                           																								//send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_CTRL_REG1);     																								//write MAG3110 control register 1
  write_byte_i2c0(0x09);     																															//write proper values
  stop_i2c0();                             																								//send stop bit, stop transmission
  disable_i2c0();                          																								//disable i2c module
}

/*----------------------------------------------------------------------------
  Function depend for read x values from MAG3110 sensor.
 *---------------------------------------------------------------------------*/
int	get_mag3110_x(void) {
	int16_t msb = 0;																																				//msb variable
	int16_t lsb = 0;																																				//lsb variable
	int16_t full_data = 0;																																	//full data, variable to indicate proper value
	int val_to_ret = 0;																																			//value to return
	enable_i2c0();                           																								//enable i2c0 module 
  start_i2c0();                            																								//start transmission, send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_OUT_X_MSB);        																							//write MAG3110 register which values msb to read
  restart_i2c0();                          																								//resatrt i2c, send restart bit
  write_byte_i2c0(MAG3110_ADDR_READ);      																								//write MAG3110 device read address
  msb = read_byte_i2c0(I2C0_NACK); 																												//read msb values
  stop_i2c0();                             																								//send stop bit, stop transmission
  disable_i2c0();                          																								//disable i2c0 modyle
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_OUT_X_LSB);        																							//write MAG3110 register which values lsb to read
  restart_i2c0();                          																								//restart transmission, send restart bit
  write_byte_i2c0(MAG3110_ADDR_READ);      																								//write MAG3110 device read address
  lsb = read_byte_i2c0(I2C0_NACK); 																												//read lsb values
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();                          																								//disable i2c0 module
	full_data = ((msb << 8) | (lsb));																												//full data
	val_to_ret = full_data;																																	//assign int16_t to ineger
	return  val_to_ret;																																			//return value
}

/*----------------------------------------------------------------------------
  Function depend for read y values from MAG3110 sensor.
 *---------------------------------------------------------------------------*/
int	get_mag3110_y(void) {
	int16_t msb = 0;																																				//msb variable
	int16_t lsb = 0;																																				//lsb variable
	int16_t full_data = 0;																																	//full data, variable to indicate proper value
	int val_to_ret = 0;																																			//value to return
	enable_i2c0();                           																								//enable i2c0 module 
  start_i2c0();                            																								//start transmission, send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_OUT_Y_MSB);        																							//write MAG3110 register which values msb to read
  restart_i2c0();                          																								//resatrt i2c, send restart bit
  write_byte_i2c0(MAG3110_ADDR_READ);      																								//write MAG3110 device read address
  msb = read_byte_i2c0(I2C0_NACK); 																												//read msb values
  stop_i2c0();                             																								//send stop bit, stop transmission
  disable_i2c0();                          																								//disable i2c0 modyle
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//send start bit
  write_byte_i2c0(MAG3110_ADDR_WRITE);     																								//write MAG3110 device write address
  write_byte_i2c0(MAG3110_OUT_Y_LSB);        																							//write MAG3110 register which values lsb to read
  restart_i2c0();                          																								//restart transmission, send restart bit
  write_byte_i2c0(MAG3110_ADDR_READ);      																								//write MAG3110 device read address
  lsb = read_byte_i2c0(I2C0_NACK); 																												//read lsb values
  stop_i2c0();                             																								//stop transmission, send stop bit
  disable_i2c0();                          																								//disable i2c0 module
	full_data = ((msb << 8) | (lsb));																												//full data
	val_to_ret = full_data;																																	//assign int16_t to ineger
	return  val_to_ret;																																			//return value
}



