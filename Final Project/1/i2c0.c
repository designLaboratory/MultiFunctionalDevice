/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    i2c0.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "i2c0.h"
/*----------------------------------------------------------------------------
  Function depend for initialize i2c0 peripherials.
 *---------------------------------------------------------------------------*/ 
void initialize_i2c0(void) {
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;																											//clock for i2c0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;																											//clock for porte where are SCL and SDA
	PORTB->PCR[2] |= PORT_PCR_MUX(2);																												//PIN B2 will be SCL
	PORTB->PCR[3] |= PORT_PCR_MUX(2);																												//PIN B3 will be SDA
	I2C0->F   = I2C_F_MULT(2) | I2C_F_ICR(0x2D);                    												//set baudrate: 9600
}

/*----------------------------------------------------------------------------
  Function depend for enable i2c0 module.
 *---------------------------------------------------------------------------*/
void enable_i2c0(void) {	  
	  I2C0->C1 |= I2C_C1_IICEN_MASK;																												//enable i2c0 module, bit IICEN set to 1
}

/*----------------------------------------------------------------------------
  Function depend for start i2c0 transmission.
 *---------------------------------------------------------------------------*/
void start_i2c0(void) {  
  I2C0->C1 |= I2C_C1_MST_MASK;																														//choose master option and send start bit
}

/*----------------------------------------------------------------------------
  Function depend for restart i2c0 transmission and generate restart bit.
 *---------------------------------------------------------------------------*/
void restart_i2c0(void) {
  I2C0->C1 |= I2C_C1_RSTA_MASK;																														//generate restart bit, restart transmission
}

/*----------------------------------------------------------------------------
  Function depend for read i2c0 values.
 *---------------------------------------------------------------------------*/
uint8_t read_byte_i2c0(uint8_t ack) {
  I2C0->C1 &= ~I2C_C1_TX_MASK;																														//device will be in receive mode																			
	if((I2C0->SMB & I2C_SMB_FACK_MASK) == 0) 																								//check FACK bit, if is no set then (hardware bit is clear)
		I2C0->C1 = (ack == I2C0_NACK) ? 																											//if nack is as argument of function
			I2C0->C1 | I2C_C1_TXAK_MASK:																												//send nack
			I2C0->C1 & ~I2C_C1_TXAK_MASK;																												//send ack   
	I2C0->S |= I2C_S_IICIF_MASK;																														//clear IICIF flag, interrupt flag 
  I2C0->D;																																								//receive and discard wrong values
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);																								//check if end of transmission and ack is received 
  I2C0->C1 |= I2C_C1_TX_MASK;																															//device will be in transmit mode 
  return I2C0->D;																																					//receive proper value
}

/*----------------------------------------------------------------------------
  Function depend for stop i2c0 transmission.
 *---------------------------------------------------------------------------*/ 
void stop_i2c0(void) {																																			
  I2C0->FLT |= I2C_FLT_STOPF_MASK;																												//clear stop flag
  I2C0->C1 &= ~I2C_C1_MST_MASK;																														//set slave option and generate stop bit
  while((I2C0->FLT & I2C_FLT_STOPF_MASK) == 0){																						//wait for stop bit set
    I2C0->C1 &= ~I2C_C1_MST_MASK;																													//set slave option and generate stop bit
  }
}

/*----------------------------------------------------------------------------
  Function depend for write data to i2c0.
 *---------------------------------------------------------------------------*/  
uint8_t write_byte_i2c0(uint8_t data) {
  I2C0->C1 |= I2C_C1_TX_MASK;																															//transmit mode
  I2C0->S |= I2C_S_IICIF_MASK;																														//clear interrupt flag 
  I2C0->D = data;																																					//write data			
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);																								//wait for end of transsmison and get ack
  return ((I2C0->S & I2C_S_RXAK_MASK) == I2C_S_RXAK_MASK ? I2C0_NACK : I2C0_ACK);					//if ack signal was received then return ACK else return NACK
}

/*----------------------------------------------------------------------------
  Function depend for disable i2c0 module.
 *---------------------------------------------------------------------------*/ 
void disable_i2c0(void) {
  I2C0->C1 &= ~I2C_C1_IICEN_MASK;																													//iicen to 0, disable i2c module
}
