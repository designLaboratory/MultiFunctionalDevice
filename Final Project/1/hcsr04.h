/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    hcsr04.h
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
  
#ifndef HCSR04_H
#define HCSR04_H

#include "MKL46Z4.h"

void initialize_hcsr04(void);
float get_hcsr04_distance(void);
void disable_hcsr04(void);
void enable_hcsr04(void);
#endif
