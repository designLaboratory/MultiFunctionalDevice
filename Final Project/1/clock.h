/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    clock.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#ifndef CLOCK_H
#define CLOCK_H

#include "MKL46Z4.h"
#include "buttons.h"

void initialize_clock(uint8_t second, uint8_t minutes, uint8_t hours);
uint8_t get_hours(void);
uint8_t get_minutes(void);
uint8_t get_seconds(void);
void add_seconds(uint16_t value);
void disable_clock(void);
void enable_clock(void);

#endif
