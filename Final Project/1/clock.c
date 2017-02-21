/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    clock.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#include "clock.h"

uint32_t seconds = 0;

/*----------------------------------------------------------------------------
  Function depends for initialize pit module, time for clock.
 *---------------------------------------------------------------------------*/ 
void initialize_clock(uint8_t second, uint8_t minutes, uint8_t hours) {
	seconds = minutes*60+second+3600*hours;																									//set time from main 
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;																												//enable clock for pit
	PIT->CHANNEL[0].LDVAL = 0xD55160;																												//variable to interrupt, interrupt every 1 s
	NVIC_ClearPendingIRQ(PIT_IRQn);																													//clear pendinf IRQ for PIT
	NVIC_EnableIRQ(PIT_IRQn);																																//set IRQ for PIT
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;																						//enable interrupts for overflow PIT channel 0
	PIT->CHANNEL[0].TCTRL |=  PIT_TCTRL_TEN_MASK;
	PIT->MCR = 0x00;																																				//module enable
}

/*----------------------------------------------------------------------------
  PIT IRQ service.
 *---------------------------------------------------------------------------*/
void PIT_IRQHandler(void) {																					
	seconds++;																																							//increment seconds
	if (seconds > 60+24*3600) seconds -= 60+24*3600;																				//check seconds if are too high
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;																							//clear interrupt flag
	PIT->CHANNEL[0].TCTRL |=  PIT_TCTRL_TEN_MASK |																					//enable timer		
														PIT_TCTRL_TIE_MASK; 																					//enable interrupts
}

/*----------------------------------------------------------------------------
  Get hours from second.
 *---------------------------------------------------------------------------*/
uint8_t get_seconds(void) {
	uint32_t temp = seconds;
  return temp % 60;                       																						
}

/*----------------------------------------------------------------------------
  Get minutes from second.
 *---------------------------------------------------------------------------*/
uint8_t get_minutes(void) {
	uint32_t temp = seconds;
	temp = temp / 60;
	return temp % 60;
}

/*----------------------------------------------------------------------------
  Get seconds from second.
 *---------------------------------------------------------------------------*/
uint8_t get_hours(void) {
	uint32_t temp = seconds;
	temp = temp / 60;
	temp = temp / 60;
	return temp % 24;
}

/*----------------------------------------------------------------------------
  Add seconds.
 *---------------------------------------------------------------------------*/
void add_seconds(uint16_t value) {
	seconds+=value;
}

/*----------------------------------------------------------------------------
  Enable clock.
 *---------------------------------------------------------------------------*/
void disable_clock(void) {
	PIT->CHANNEL[0].TCTRL &=  ~PIT_TCTRL_TEN_MASK;
}

/*----------------------------------------------------------------------------
  Disable clock.
 *---------------------------------------------------------------------------*/
void enable_clock(void) {
	PIT->CHANNEL[0].TCTRL |=  PIT_TCTRL_TEN_MASK;
}
