/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart0.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
#include "uart0.h"

/*----------------------------------------------------------------------------
  Function depend for initialize UART0 peripherial.
 *----------------------------------------------------------------------------*/
void initialize_uart0(void) {	
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;																											//assign clock for UART0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;																											//assign clock for PORTA, PTA1 - UART0RX, PTA2 - UART0TX
	PORTA->PCR[1] |= PORT_PCR_MUX(2);																												//PTA-1 will be UART0-RX
	PORTA->PCR[2] |= PORT_PCR_MUX(2);																												//PTA-2 will be UART0-TX
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);																										//clock for uart 42 MHz
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);																		//block transmitter and receiver
	UART0->C4 |= UART0_C4_OSR(31);																													//oversampling ratio 31
	UART0->BDH |= UART_BDH_SBR(0);																													//value of SBR is 137
	UART0->BDL |= UART_BDL_SBR(137);																												
	UART0->BDH &= ~(UART0_BDH_SBNS_MASK);																										//one stop bit
	UART0->C1 &= ~(UART0_C1_PE_MASK | UART0_C1_M_MASK);																			//no parity checking and 8 bit value
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);																			//enable receiver and transmitter
}

/*----------------------------------------------------------------------------
  Read byte from UART0.
 *----------------------------------------------------------------------------*/
uint8_t read_byte_uart0(void) {
  uint8_t r = 0; 																							
  while( !(UART0->S1 & UART_S1_RDRF_MASK) );																							//wait for receiver is full
  r = UART0->D;																																						//read byte from receiver
  return r;																																								//and return it
}

/*----------------------------------------------------------------------------
  Write byte from UART0.
 *----------------------------------------------------------------------------*/
void write_byte_uart0(uint8_t byte) {
  while( !(UART0->S1 & UART_S1_TDRE_MASK) );																							//check if transmitter is empty
  UART0->D = byte;																																				//write byte to transmitter
}

/*----------------------------------------------------------------------------
  Function preparing data to send.
 *----------------------------------------------------------------------------*/
void data_to_send(int32_t data, int8_t function) {
	char tab[8] = {'0','0','0','0','0','0','0','0'};
	uint8_t set_pause = 0;	
	int8_t i = 0;
	if (data < 0) {
		data *= (-1);
		set_pause = 1;
	}
	while(data) {																																						//change value to decimal substracts
		tab[i++]=data%10;
		data/=10;
	}	
	tab[7] = function;																																			//assign function indicator
	for (i = 0; i < 8; i++) {																																//change uint8_t to char
		switch(tab[i]) {
			case 0: tab[i] = '0';
							break;							
			case 1: tab[i] = '1';
							break;
			case 2: tab[i] = '2';
							break;
			case 3: tab[i] = '3';
							break;
			case 4: tab[i] = '4';
							break;
			case 5: tab[i] = '5';
							break;
			case 6: tab[i] = '6';
							break;
			case 7: tab[i] = '7';
							break;
			case 8: tab[i] = '8';
							break;
			case 9: tab[i] = '9';
							break;
			default:
							tab[i] = '0';
							break;			
		}
	}																																												//send values in proper order
	if (set_pause) tab[2] = '-';																															// - sign
	for (i = 7; i >= 0; i--) {																											
		write_byte_uart0(tab[i]);																																
	}
}



