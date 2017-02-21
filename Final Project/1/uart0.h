/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart0.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
  
#ifndef UART0_H
#define UART0_H

#include "MKL46Z4.h"

void initialize_uart0(void);
uint8_t read_byte_uart0(void);
void write_byte_uart0(uint8_t byte);
void data_to_send(int32_t data, int8_t function);

#endif
