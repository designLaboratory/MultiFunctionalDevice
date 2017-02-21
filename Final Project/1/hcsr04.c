/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    hcsr04.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#include "hcsr04.h"

uint32_t tpm2_value = 0;																																	//latched timer value
uint16_t difference = 0;																																	//diference between rising and faling edge on tpm2
uint8_t i = 0;																																						//index of probes_table

/*----------------------------------------------------------------------------
  Probe from sensor.
 *---------------------------------------------------------------------------*/
float get_hcsr04_distance(void) {																														
	return difference * 0.0259;
}

/*----------------------------------------------------------------------------
  TMP2_IRQHandler.
 *---------------------------------------------------------------------------*/
void TPM2_IRQHandler(void) {
	uint32_t portb_stat	= (FPTB->PDIR & (1UL << 18));																				//state od PTB18 register, use fast GPIO
	if (portb_stat != 0) {																																	//if rising edge on PTB18
		tpm2_value = TPM2->CNT;																																//assign latched tpm2 state
		TPM2->SC |= TPM_SC_TOF_MASK;																													//clear overflow tpm2
		TPM1->SC |= TPM_SC_CMOD(0);																														//disable tpm1
	} else if (portb_stat == 0)  {																													//if falling edge on PTB18
		if ((TPM2->SC & TPM_SC_TOF_MASK) == 0) {																							//check if tmp is not overflowed
			difference = TPM2->CNT - tpm2_value;																								//get diference
			disable_hcsr04();																																		//disable sensor
		}
		TPM1->CNT = 0;																																				//clear tpm1
		TPM1->SC |= TPM_SC_CMOD(1);																														//enable tpm1	
	}
	TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK; 																						//clear channel event flag
}

/*----------------------------------------------------------------------------
  Initialize peripherials for HCSR04 sensor.
 *---------------------------------------------------------------------------*/
void initialize_hcsr04(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;																											//clock for portb 	
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;																											//clock for tpm1 module
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;																											//clock for tpm2 module
	PORTB->PCR[1] |= PORT_PCR_MUX(3);  																											//pin B1 is tpm1 output
	PORTB->PCR[18] |= PORT_PCR_MUX(3);																												//pin B19 is tpm2 output
	TPM1->SC |= TPM_SC_PS(7);																																//prescaler for 128 -> 328125 Hz -> 3.04 us
	TPM2->SC |= TPM_SC_PS(6);																																//prescaler for 128 -> 328125 Hz -> 3.04 us
	TPM1->CNT = 0;																																					//reset TPM1 counter
	TPM2->CNT = 0;																																					//reset TPM2 counter
	TPM1->SC |= TPM_SC_CPWMS_MASK;																													//up-counting mode	
	TPM2->SC &= ~TPM_SC_CPWMS_MASK;																													//up-counting mode
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK																						//PWM Center Eligned High true pulses
														| TPM_CnSC_ELSA_MASK);	
	TPM1->CONTROLS[1].CnSC &= (~TPM_CnSC_MSA_MASK
														| ~TPM_CnSC_ELSB_MASK);	
	
	TPM2->CONTROLS[0].CnSC &=  ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);										//interrupt when rising or falling edge at input
	TPM2->CONTROLS[0].CnSC |=  (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);	
	TPM2->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK; 																					//enable interrupt from channel 0
	NVIC_ClearPendingIRQ(TPM2_IRQn);																												//enable NVIC, clear pending irq 
	NVIC_SetPriority(TPM2_IRQn,1);																													//set priority
	NVIC_EnableIRQ(TPM2_IRQn);																															//set NVIC interrupt from TPM2_IRQn
	TPM1->MOD = 8191;																																				//~100ms of trigger cycle
	TPM1->CONTROLS[1].CnV = 8189;																														//change from 1 to 0 after ~ 15us, clear after timer clear ~ 200 ms
}

/*----------------------------------------------------------------------------
  Enable timers.
 *---------------------------------------------------------------------------*/
void enable_hcsr04(void) {
	TPM1->SC |= TPM_SC_CMOD(1);																															//enable tpm1
	TPM2->SC |= TPM_SC_CMOD(1);																															//enable tpm2
}

/*----------------------------------------------------------------------------
  Disable timers, no interrupts from TPM2.
 *---------------------------------------------------------------------------*/
void disable_hcsr04(void) {
	TPM1->SC |= TPM_SC_CMOD(0);																															//disable tpm1
	TPM2->SC |= TPM_SC_CMOD(0);																															//disable tpm2
}
