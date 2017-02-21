#include "MKL46Z4.h"
#include "buttons.h"
#include "diodes.h"
#include "i2c0.h"
#include "uart0.h"
#include "ws2812b.h"
#include "mma8451q.h"
#include "mag3110.h"

#define LEDS_IN_STRIP 24
#define LAB_VIEW_SUPPORT 1
#define MODULO_ANGLE 30

static uint8_t calibration = 0;																																						//indicate if calibration is finished or no
static int average_axis_x = 0;		
static int average_axis_y = 0;	

int main(void) {
	int temp_x_max, temp_x_min, temp_y_max, temp_y_min, new_x, new_y;																				//variable for average position
	int mag3110_x = 0;																																											//magnetometr x axis value
	int mag3110_y = 0;
	int16_t mma8451q_x = 0;
	int16_t mma8451q_y = 0;
	int16_t mma8451q_z = 0;
	int32_t acc_angles = 0;
	volatile int32_t xtoz = 0;
	volatile int32_t ytoz = 0;
	uint32_t i = 0;
	uint32_t j = 0;	
	initialize_i2c0();
	initialize_button();
	initialize_uart0();
	initialize_ws2812b();	
	initialize_mag3110();	
	initialize_mma8451q();
	while(1) {
		switch(return_function()) {
			case 8:		if (calibration == 0) {
									average_axis_x = 0;																																			//set global average variable to 0
									average_axis_y = 0;																																			//set global average variable to 0
									for (i = 0; i < 24; i++) {																															//set diodes to begin calibration
										set_pixel_color(i,60,60,60);																													//set pixel_table
									}	
									i = 0;	
									j = 0;														
									temp_x_max = temp_x_min = get_mag3110_x();																							//read values from MAG3110 and assing to proper variables
									temp_y_max = temp_y_min = get_mag3110_y();																							//read values from MAG3110 and assign to proper variables
  								while (j < 24) {																																				//while i<24 to set 24 diodes
										while(i < 1000) {																																			//loop
											new_x = get_mag3110_x();																														//read new x variable
											new_y = get_mag3110_y();																														//read new y variable		
											if (new_x > temp_x_max) temp_x_max = new_x;																					//max from x variables
											if (new_x < temp_x_min) temp_x_min = new_x;																					//min from x variables
											if (new_y > temp_y_max) temp_y_max = new_y;																					//max from y variables
											if (new_y < temp_y_min) temp_y_min = new_y;																					//min from y variables
											i++;																																								//increment i
										}		
										i = 0;																																								//set i to 0
										set_pixel_color(j,0,60,0);																														//set j diode which indicate progress of calibration
										start_dma();																																					//start dma, display diodes in ring
										j++;																																									
									}
									j = 0;																																									//increment j																																									
									average_axis_x = (temp_x_max+temp_x_min)/2;																							//assign average measure variables to global variables
									average_axis_y = (temp_y_max+temp_y_min)/2;	
									calibration = 1;
								}
								for (i = 0; i < 24; i++) {																																//set all diodes to green color
									set_pixel_color(i,0,60,0);
								}							
								mag3110_x = get_mag3110_x();																															//read values from sensor
								mag3110_y = get_mag3110_y();																															//read values from sensor
								set_pixel_color(get_diode_mag3110(mag3110_x,mag3110_y,average_axis_x,average_axis_y),0,0,60); 																	//set proper diodes
								set_pixel_color(get_diode_mag3110(mag3110_x,mag3110_y,average_axis_x,average_axis_y)+12,0,0,60); 																//set proper diodes
								if (LAB_VIEW_SUPPORT) data_to_send(return_angle_mag3110(mag3110_x,mag3110_y,average_axis_x,average_axis_y),7);
								break;
			case 9: 	mma8451q_x = get_mma8451q_x();
								mma8451q_y = get_mma8451q_y();
								mma8451q_z = get_mma8451q_z();
								xtoz = get_degree_mma8451q(mma8451q_x,mma8451q_z);
								ytoz = get_degree_mma8451q(mma8451q_y,mma8451q_z);
								for (i = 0; i < LEDS_IN_STRIP; i++) {																												//default, all diodes will be white
									set_pixel_color(i,0,60,0);																										
								}
								for (i = 0; i < ret_how(xtoz,MODULO_ANGLE); i++) {																					//set proper values in pixel tables, Kat przechylenia (na boki) 
									set_pixel_color(i,0,0,60);
								}
								if (ret_brightness(xtoz,MODULO_ANGLE)) {																										//brightness of last diode
									set_pixel_color(ret_how(xtoz,MODULO_ANGLE),0,0,2*ret_brightness(xtoz,MODULO_ANGLE));
								} 							
								for (i = 12; i < 12+ret_how(ytoz,MODULO_ANGLE); i++) {																			//set proper values in pixel tables,Kat przechylenia (w przód i tyl)
									set_pixel_color(i,60,0,0);
								}
								if (ret_brightness(ytoz,MODULO_ANGLE)) {																										//brightness of last diode
									set_pixel_color(12+ret_how(ytoz,MODULO_ANGLE),2*ret_brightness(ytoz,MODULO_ANGLE),0,0);
								}
								if (LAB_VIEW_SUPPORT) {
									acc_angles = xtoz*1000+ytoz;
									data_to_send(acc_angles,8); 					
								}
								break;
			default:	for (i = 0; i < LEDS_IN_STRIP; i++) {																												//default, all diodes will be white
									set_pixel_color(i,60,60,60);																										
								}
								break;
			}
		start_dma();	
	}
}
