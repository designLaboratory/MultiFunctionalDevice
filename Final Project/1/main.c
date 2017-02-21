#include "MKL46Z4.h"
#include "i2c0.h"
#include "bme280.h"
#include "ws2812b.h"
#include "hcsr04.h"
#include "uart0.h"
#include "math.h"
#include "diodes.h"
#include "buttons.h"
#include "clock.h"

#define LEDS_IN_STRIP 24
#define LAB_VIEW_SUPPORT 1
#define MODULO_DISTANCE 10
#define MODULO_TEMPERATURE 3
#define MODULO_HUMADITY 9
#define MODULO_PRESSURE 9
#define MODULO_SECONDS 5
#define MODULO_MINUTES 5
#define NUMBER_OF_SAMPLES 24




/*----------------------------------------------------------------------------
  Main function.
 *---------------------------------------------------------------------------*/
int main(void) {	
	uint8_t end_of_first_sampling = 0;
	uint8_t end_of_second_sampling = 0;	
	uint16_t i = 0;
	int8_t hours = 12;
	int8_t seconds = 30;
	int8_t minutes = 30;
	int32_t first_stage = 0;
	int32_t second_stage = 0;
	int16_t temperature = 0;
	int16_t humadity = 0;	
	int16_t distance = 0;
	int16_t pressure = 0;
	int32_t difference = 0;
	initialize_i2c0();																																				//initialize i2c0 module
	initialize_bme280();																																			//initialize and configure bme280 sensor
	initialize_ws2812b();																																			//initialize ws2812b diodes
	initialize_uart0();																																				//initialize uart0
	initialize_hcsr04();																																			//initalize HCSR04 sensor
	initialize_button();																																			//initialize buttons
	initialize_clock(seconds,minutes,hours);																									//initialize pit clock
	while(1) {
		switch(return_function()) {																															//switch function which is depend from button clicks
			case 1:		for (i = 0; i < LEDS_IN_STRIP; i++) {																				//standard values in pixel table
									set_pixel_color(i,0,60,0);
								}	
								enable_hcsr04();																														//case 1 -> distance sensor, enable hcsr04 timers
								delay_mc(100);																															//wait for some samples
								distance = floor(get_hcsr04_distance());																		//floor distance
								for (i = 0; i < ret_how(distance,MODULO_DISTANCE); i++) {										//set proper values in pixel tables
									set_pixel_color(i,0,0,60);
								}
								if (ret_brightness(distance,MODULO_DISTANCE)) {															//brightness of last diode
									set_pixel_color(ret_how(distance,MODULO_DISTANCE),0,0,6*ret_brightness(distance,MODULO_DISTANCE));
								} 
								if (LAB_VIEW_SUPPORT) data_to_send(distance,1);															//send values by uart0
								break;
			case 2:		for (i = 0; i < LEDS_IN_STRIP; i++) {																				//case 2 -> temperature sensor
									set_pixel_color(i,0,60,0);
								}	
								temperature = floor(get_bme280_temperature());															//get i2c0 values from sensor
								if (temperature == 0) {
									set_pixel_color(0,20,0,0);	
									set_pixel_color(12,0,0,20);
								} else if (temperature > 0) {																								//set proper diodes
									for (i = 0; i < ret_how(temperature,MODULO_TEMPERATURE); i++) {						//set proper values in pixel tables
										set_pixel_color(i,60,0,0);
									}
									if (ret_brightness(temperature,MODULO_TEMPERATURE)) {											//brightness of last diode
										set_pixel_color(ret_how(temperature,MODULO_TEMPERATURE),20*ret_brightness(temperature,MODULO_TEMPERATURE),0,0);
									}	 			
								} else if (temperature < 0) {
									for (i = 12; i < ret_how(temperature,MODULO_TEMPERATURE)+12; i++) {				//set proper values in pixel tables
										set_pixel_color(i,0,0,60);
									}
									if (ret_brightness(temperature,MODULO_TEMPERATURE)) {											//brightness of last diode
										set_pixel_color(ret_how(temperature,MODULO_TEMPERATURE)+12,0,0,20*ret_brightness(temperature,MODULO_TEMPERATURE));
									}	 		
								}								
								if (LAB_VIEW_SUPPORT) data_to_send(temperature,2);													//send values by uart0
								break;								
			case 3:		for (i = 0; i < LEDS_IN_STRIP; i++) {																				//case 2 -> humadity sensor
									set_pixel_color(i,0,60,0);
								}	
								humadity = floor(get_bme280_humadity());																		//get humadity values and multiply by 100 to get values from 0 to 100
								for (i = 0; i < ret_how(humadity,MODULO_HUMADITY); i++) {										//set proper values in pixel tables
									set_pixel_color(i,0,0,60);
								}
								if (ret_brightness(humadity,MODULO_HUMADITY)) {															//brightness of last diode
									set_pixel_color(ret_how(humadity,MODULO_HUMADITY),0,0,7*ret_brightness(humadity,MODULO_HUMADITY));
								} 
								for (i = 12; i < LEDS_IN_STRIP; i++) {																			//diodes in small ring will be white															
									set_pixel_color(i,60,60,60);
								}
								if (LAB_VIEW_SUPPORT) data_to_send(humadity,3);															//send values by uart0
								break;
			case 4:		for (i = 0; i < LEDS_IN_STRIP; i++) {																				//case 4 -> pressure sensor
									set_pixel_color(i,0,60,0);
								}	
								pressure = floor(get_bme280_pressure());																		//humadity will be from 945 Hpa to 1045Hpa
								pressure -= 945;																														//to get values from 0 to 100
								for (i = 0; i < ret_how(pressure,MODULO_PRESSURE); i++) {										//set proper values in pixel tables
									set_pixel_color(i,0,0,60);
								}
								if (ret_brightness(pressure,MODULO_PRESSURE)) {															//brightness of last diode
									set_pixel_color(ret_how(pressure,MODULO_PRESSURE),0,0,7*ret_brightness(pressure,MODULO_PRESSURE));
								} 
								for (i = 12; i < LEDS_IN_STRIP; i++) {																			//diodes in small ring will be white
									set_pixel_color(i,60,60,60);
								}
								pressure += 945;																														//to send proper values by uart0
								if (LAB_VIEW_SUPPORT) data_to_send(pressure,4);															//send values by uart0
								break;
			case 5:		for (i = 0; i < LEDS_IN_STRIP; i++) {																				//case 5 -> clock
									set_pixel_color(i,50,50,50);
								}												
								hours = get_hours();																												//get time values
								minutes = get_minutes();
								seconds = get_seconds();
								if (hours >= 12) {																													//check if is pm or am, if pm then set hours diodes to green color
									set_pixel_color(0,0,50,0);																								//hours indicators diodes
									set_pixel_color(3,0,50,0);
									set_pixel_color(6,0,50,0);
									set_pixel_color(9,0,50,0);			
									set_pixel_color(hours-12,50,50,0);																				//hour tip in yellow color
								} else {																																		//if am then set hours diodes to blue color
									set_pixel_color(0,0,0,50);																								//hours indicators diodes
									set_pixel_color(3,0,0,50);
									set_pixel_color(6,0,0,50);
									set_pixel_color(9,0,0,50);			
									set_pixel_color(hours,50,50,0);																						//hour tip in yellow color
								}			
								for (i = 12; i < LEDS_IN_STRIP; i++) {																			//seconds diodes to blue color
									set_pixel_color(i,0,0,50);
								}
								for (i = 12; i < ret_how(seconds,MODULO_SECONDS)+12; i++) {									//set proper values in pixel tables
									set_pixel_color(i,50,0,0);
								}
								if (ret_brightness(seconds,MODULO_SECONDS)) {																//brightness of last diode
									set_pixel_color(ret_how(seconds,MODULO_SECONDS)+12,10*ret_brightness(seconds,MODULO_SECONDS),0,0);
								} 
								if (ret_brightness(minutes,MODULO_MINUTES) == 0) {													//if bbrightness 0 ten
									set_pixel_color(ret_how(minutes,MODULO_MINUTES),10,0,0);									//set in lowest brighhtness
								} else {																																		//else 
									set_pixel_color(ret_how(minutes,MODULO_MINUTES),(ret_brightness(minutes,MODULO_MINUTES))*10,0,0);										
								}							
								if (LAB_VIEW_SUPPORT) data_to_send(minutes*60+seconds+3600*hours,5);				//send values by uart0
								break;
			case 6:		end_of_second_sampling = 0;																									//case 6 -> samples of piedestal, 480 samples
								for (i = 0; i < LEDS_IN_STRIP; i++) {																				
									set_pixel_color(i,0,60,0);
								}
								if (end_of_first_sampling == 0) {																						//sampling must be once time
									gaming_sensor_mode();									
									clear_probes_table();						
									for (i = 0; i < LEDS_IN_STRIP; i++) {																				
										set_pixel_color(i,60,60,60);
									}									
									for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
										assign_value(i,get_bme280_pressure());
										set_pixel_color(i,0,60,0);
										delay_mc(100);
										start_dma();
									}
								}
								end_of_first_sampling = 1;																						
								first_stage = floor(100*average_samples());																	//get average of samples
								break;
			case 7:		end_of_first_sampling = 0;																									//case 7 -> samples of second stage, 480 samples
								for (i = 0; i < LEDS_IN_STRIP; i++) {																				
									set_pixel_color(i,60,0,0);
								}
								if (end_of_second_sampling == 0) {																					//sampling must be once time
									clear_probes_table();
									for (i = 0; i < LEDS_IN_STRIP; i++) {																				
										set_pixel_color(i,60,60,60);
									}
									for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
										assign_value(i,get_bme280_pressure());
										set_pixel_color(i,60,0,0);										
										delay_mc(100);
										start_dma();
									}
								}
								end_of_second_sampling = 1;
								second_stage = floor(100*average_samples());																//get average of samples
								normal_sensor_mode();
								difference = first_stage - second_stage;																		//case 8 -> show difference
								difference -= 12;
								for (i = 0; i < LEDS_IN_STRIP; i++) {																				
									set_pixel_color(i,0,60,0);
								}
								if (difference > 0) {
									for (i = 0; i < difference; i++) {																				
										set_pixel_color(i,0,0,60);
									}
								} else if (difference < 0) {									
									for (i = 0; i < (difference*(-1)); i++) {																				
										set_pixel_color(i,60,0,0);
									}
								} else {
									for (i = 0; i < LEDS_IN_STRIP; i++) {																				
										set_pixel_color(i,0,60,0);
									}
								}							
								if (LAB_VIEW_SUPPORT) data_to_send(difference,6);
								break;
								
			default:	for (i = 0; i < LEDS_IN_STRIP; i++) {																				//default, all diodes will be green
									set_pixel_color(i,60,60,60);																										
								}
								break;
			}
		start_dma();																																						//start dma transfer, enable diodes
	}	
}

