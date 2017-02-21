/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    mma8451q.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#include "mma8451q.h"
#include "i2c0.h"

#define MMA8451Q_ADDR_WRITE 0x3A																													//predefiniton with i2c write adress of MMA8451Q																				
#define MMA8451Q_ADDR_READ 0x3B																														//predefiniton with i2c write adress of MMA8451Q
#define MMA8451Q_OUT_X_MSB 0x01																														//predefiniton with i2c out data from x asis msb register adress in MMA8451Q
#define MMA8451Q_OUT_X_LSB 0x02																														//predefiniton with i2c out data from x asis lsb register adress in MMA8451Q
#define MMA8451Q_OUT_Y_MSB 0x03																														//predefiniton with i2c out data from y asis msb register adress in MMA8451Q	
#define MMA8451Q_OUT_Y_LSB 0x04																														//predefiniton with i2c out data from y asis lsb register adress in MMA8451Q
#define MMA8451Q_OUT_Z_MSB 0x05																														//predefiniton with i2c out data from z asis msb register adress in MMA8451Q
#define MMA8451Q_OUT_Z_LSB 0x06																														//predefiniton with i2c out data from z asis lsb register adress in MMA8451Q
#define MMA8451Q_CTRL_REG1 0x2A																														//predefiniton with i2c control register

/*----------------------------------------------------------------------------
  Function depend for adjust accelerometr, assign values to control 
	registers.
 *---------------------------------------------------------------------------*/  
void initialize_mma8451q(void) {
  enable_i2c0(); 																																					//enable i2c0 module                          																								
  start_i2c0();                            																								//start i2c0 transmission
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write magnetometr write address
  write_byte_i2c0(MMA8451Q_CTRL_REG1);     																								//write magnetometr control 1 registers
  write_byte_i2c0(0x1D);     																															//write value to this register
  stop_i2c0();               																															//stop i2c0 transmission              
  disable_i2c0();  																																				//disable i2c0 module
}

/*----------------------------------------------------------------------------
  Function depend for read x values from MMA8451Q sensor.
 *---------------------------------------------------------------------------*/  
int get_mma8451q_x(void) {
	int16_t msb = 0;																																				//msb variable
	int16_t lsb = 0;																																				//lsb variable
	int16_t full_data = 0;																																	//full data, variable to indicate proper value
	int val_to_ret = 0;																																			//value to return
	enable_i2c0();                           																								//enable i2c0 module 
  start_i2c0();                           																								//start transmission, send start bit
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_X_MSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address
  msb = read_byte_i2c0(I2C0_NACK); 																												//read msb
  stop_i2c0();                             																								//stop i2c0 transmission, send stop bit
  disable_i2c0();                   																											//disable i2c0 module       
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start i2c0 module
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_X_LSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address	
  lsb = read_byte_i2c0(I2C0_NACK); 																												//read lsb
  stop_i2c0();                             																								//stop i2c0 transmission
  disable_i2c0();                          																								//disable i2c0 module
	full_data = ((msb << 8) | (lsb));																												//full data
	full_data /= 4;																																					//divide by 4 to get proper data
	val_to_ret = full_data;																																	//assign int16_t to ineger
	return  val_to_ret;																																			//return value
}

/*----------------------------------------------------------------------------
  Function depend for read y values from MMA8451Q sensor.
 *---------------------------------------------------------------------------*/  
int get_mma8451q_y(void) {
	int16_t msb = 0;																																				//msb variable
	int16_t lsb = 0;																																				//lsb variable
	int16_t full_data = 0;																																	//full data, variable to indicate proper value
	int val_to_ret = 0;																																			//value to return
	enable_i2c0();                           																								//enable i2c0 module 
  start_i2c0();                           																								//start transmission, send start bit
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_Y_MSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address
  msb = read_byte_i2c0(I2C0_NACK); 																												//read msb
  stop_i2c0();                             																								//stop i2c0 transmission, send stop bit
  disable_i2c0();                   																											//disable i2c0 module       
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start i2c0 module
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_Y_LSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address	
  lsb = read_byte_i2c0(I2C0_NACK); 																												//read lsb
  stop_i2c0();                             																								//stop i2c0 transmission
  disable_i2c0();                          																								//disable i2c0 module
	full_data = ((msb << 8) | (lsb));																												//full data
	full_data /= 4;																																					//divide by 4 to get proper data
	val_to_ret = full_data;																																	//assign int16_t to ineger
	return  val_to_ret;																																			//return value
}

/*----------------------------------------------------------------------------
  Function depend for read z values from MMA8451Q sensor.
 *---------------------------------------------------------------------------*/  
int get_mma8451q_z(void) {
	int16_t msb = 0;																																				//msb variable
	int16_t lsb = 0;																																				//lsb variable
	int16_t full_data = 0;																																	//full data, variable to indicate proper value
	int val_to_ret = 0;																																			//value to return
	enable_i2c0();                           																								//enable i2c0 module 
  start_i2c0();                           																								//start transmission, send start bit
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_Z_MSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address
  msb = read_byte_i2c0(I2C0_NACK); 																												//read msb
  stop_i2c0();                             																								//stop i2c0 transmission, send stop bit
  disable_i2c0();                   																											//disable i2c0 module       
	enable_i2c0();                           																								//enable i2c0 module
  start_i2c0();                            																								//start i2c0 module
  write_byte_i2c0(MMA8451Q_ADDR_WRITE);     																							//write accelerometr write address
  write_byte_i2c0(MMA8451Q_OUT_Z_LSB);        																						//write accelerometr read register address
  restart_i2c0();                          																								//restart i2c module
  write_byte_i2c0(MMA8451Q_ADDR_READ);      																							//write accelerometr read address	
  lsb = read_byte_i2c0(I2C0_NACK); 																												//read lsb
  stop_i2c0();                             																								//stop i2c0 transmission
  disable_i2c0();                          																								//disable i2c0 module
	full_data = ((msb << 8) | (lsb));																												//full data
	full_data /= 4;																																					//divide by 4 to get proper data
	val_to_ret = full_data;																																	//assign int16_t to ineger
	return  val_to_ret;																																			//return value
}
