/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    buttons.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#include "clock.h"

uint8_t function = 1;

/*----------------------------------------------------------------------------
  Return function of which will be display in ring.
 *---------------------------------------------------------------------------*/
uint8_t return_function(void) {
	return function;
}

/*----------------------------------------------------------------------------
  Function depend for service interrupt from PORTD or PORTC.
 *---------------------------------------------------------------------------*/
void PORTC_PORTD_IRQHandler(void) {							
	uint32_t pin_1 = PORTC->PCR[3];																													//button masks
	uint32_t pin_2 = PORTC->PCR[12];
	uint32_t pin_3 = PORTD->PCR[4];	
	if ((pin_1 & PORT_PCR_ISF_MASK) != 0) {																									//check which button is click
		add_seconds(3600);
	} else if ((pin_2 & PORT_PCR_ISF_MASK) != 0) {
		add_seconds(60);	
	} else if ((pin_3 & PORT_PCR_ISF_MASK) != 0) {
		function++;																																						//if button on ring then assign new value to function
		if (function == 8) function = 1;																											//if function is 4 then 0
	}
	PORTD->PCR[4] |= PORT_PCR_ISF_MASK; 																										//clear interrupt flag
	PORTC->PCR[3] |= PORT_PCR_ISF_MASK; 																										//clear interrupt flag
	PORTC->PCR[12] |= PORT_PCR_ISF_MASK; 																										//clear interrupt flag
}

/*----------------------------------------------------------------------------
  Function depend for initialize ports for buttons.
 *---------------------------------------------------------------------------*/
void initialize_button(void) {
	function = 1;																																						//set function to 1
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;																											//clock for portd
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;																											//clock for ptc
	PORTD->PCR[4] |= PORT_PCR_MUX(1);      																									//PTD4 is GPIO
	PORTC->PCR[3] |= PORT_PCR_MUX(1);      																									//Pin PTC3 is GPIO 
	PORTC->PCR[12] |= PORT_PCR_MUX(1);       																								//Pin PTC12 is GPIO 
	PORTD->PCR[4] |= (PORT_PCR_PE_MASK |   																									//set bit in PCR register to enable pull resistor
										PORT_PCR_PS_MASK);																										//set bit in PCR register to select pull up
	PORTC->PCR[3] |= (PORT_PCR_PE_MASK |   																									//set bit in PCR register to enable pull resistor
										PORT_PCR_PS_MASK);																										//set bit in PCR register to select pull up	
	PORTC->PCR[12] |= (PORT_PCR_PE_MASK |  																									//set bit in PCR register to enable pull resistor
											PORT_PCR_PS_MASK);																									//set bit in PCR register to select pull up	
	PORTC->PCR[3] |= 	PORT_PCR_IRQC(10);																										//set value for IRQC bit field in PCR register to select falling edge interrupts for PORTC3
	PORTC->PCR[12] |= 	PORT_PCR_IRQC(10);																									//set value for IRQC bit field in PCR register to select falling edge interrupts for PORTC12
	PORTD->PCR[4] |= 	PORT_PCR_IRQC(10);																										//set value for IRQC bit field in PCR register to select falling edge interrupts for PORTD4
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);																									//clear NVIC any pending interrupts on PORTC_D 
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);																												//enable NVIC interrupts source for PORTC_D module 
}

