/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    i2c0.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#ifndef I2C0_H
#define I2C0_H

#include "MKL46Z4.h"

#define I2C0_ACK 0																																				//predefiniton of ACK - 0
#define I2C0_NACK 1																																				//predefiniton of NACK - 1

void initialize_i2c0(void);																																
void enable_i2c0(void);																																		
void start_i2c0(void);																																					
void restart_i2c0(void);																																		
uint8_t read_byte_i2c0(uint8_t ack);																												
void stop_i2c0(void);																																			
void disable_i2c0(void);																																		
uint8_t write_byte_i2c0(uint8_t data);																											
#endif
