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
uint8_t get_diode_mag3110(int x_value, int y_value, int average_axis_x, int average_axis_y);
int32_t return_angle_mag3110(int x_value, int y_value, int average_axis_x, int average_axis_y);
int32_t get_degree_mma8451q(int val_1, int val_2);

#endif
