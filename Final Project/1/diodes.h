/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    diodes.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef DIODES_H
#define DIODES_H

#include "MKL46Z4.h"

uint8_t ret_how(int16_t value, uint8_t modulo);
uint8_t ret_brightness(int16_t value, uint8_t modulo);
void delay_mc(uint32_t value);
void clear_probes_table(void);
void assign_value(uint16_t index, float sample);
float average_samples(void);

#endif
