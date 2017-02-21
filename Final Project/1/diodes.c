/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    diodes.c
 *      Purpose: Design laboratory project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
																																													
 #include "diodes.h"
 #include "math.h"
 
 #define PROBES 24
#define PI 3.1415

static float probes_table[PROBES];
 
 /*----------------------------------------------------------------------------
  Brightness of last significant diode.
 *---------------------------------------------------------------------------*/
uint8_t ret_brightness(int16_t value, uint8_t modulo) {
	uint8_t brightness = 0;			
	if (value < 0) {																																						//check if value is greather or smaller than 0 or equal and get modulo
		brightness = (value*(-1)) % modulo;
	} else if (value > 0) {
		brightness = value % modulo;
	} else {
		brightness = 0;
	}		
	return brightness;																																					//return brightness of last significant diode
} 

 /*----------------------------------------------------------------------------
  How much diodes set.
 *---------------------------------------------------------------------------*/
uint8_t ret_how(int16_t value, uint8_t modulo) {
	uint8_t how_much = 0;																																				//check how much diodes will be set
	if (value < 0) {																																						//check if value is greather or smaller than 0 or equal and get modulo
		how_much = (((value*(-1)) - ((value*(-1)) % modulo))/modulo);	
	} else if (value > 0) {
		how_much = ((value - (value % modulo))/modulo);	
	} else {
		how_much = 0;
	}		
	return how_much;																																						//return how much diodes
}

/*----------------------------------------------------------------------------
  Delay function.
 *---------------------------------------------------------------------------*/
void delay_mc(uint32_t value) {
	uint32_t delay, x;																																					
	for(x=0; x < value; x++){																																		//delay loop
		for(delay=0; delay < 10000; delay++){};
	}
}

/*----------------------------------------------------------------------------
  Clear probes sample table.
 *---------------------------------------------------------------------------*/
void clear_probes_table(void) {
	uint16_t i = 0;
	for(i=0;i<PROBES;i++) {
		probes_table[i] = 0;
	}
}

/*----------------------------------------------------------------------------
  Assign new value to probes table.
 *---------------------------------------------------------------------------*/
void assign_value(uint16_t index, float sample) {
	probes_table[index] = sample;
}

/*----------------------------------------------------------------------------
  Average samples.
 *---------------------------------------------------------------------------*/
float average_samples(void) {
	float sum = 0;
	uint16_t i = 0;
	for (i = 0; i < PROBES; i++) {
		sum += probes_table[i];		
	}
	return sum/PROBES;
}

/*----------------------------------------------------------------------------
  Function depend for return diode wchich indicate microcontroler position.
 *---------------------------------------------------------------------------*/
uint8_t get_diode_mag3110(int x_value, int y_value, int average_axis_x, int average_axis_y) {
	uint8_t diode = 0;																																				//diode variable
	uint8_t modulo = 0;																																				//modulo variable
	int heading_degrees_int = 0;
	float degree = 0;																																					//degree in float
	float declination_angle = 0;																															//declination angle, it must be add to resault degree
	float heading_degrees = 0;																																//heading degrees indicate,
	degree = (atan2((y_value-average_axis_y),(x_value-average_axis_x)));											//degree, indicate position which show direction 
	declination_angle = (5.0 + (8.0 / 60.0)) / (180/PI);																			//declination angle value, for me is 5 degrees and 8 minutes
	degree += declination_angle;																															//add declinationg anglee do degree	
  if (degree < 0)																																						//value correction to get values from 0 to 2*pi																																		
  {
    degree += 2 * PI;
  } 
  if (degree > 2 * PI)
  {
    degree -= 2 * PI;
  }
	heading_degrees = degree * 180/PI;																												//heading degrees conversion from radian to degrees
	heading_degrees = floor(heading_degrees);																									//floor variable
	heading_degrees_int = heading_degrees;																										//convert to integer
	modulo = heading_degrees_int % 30;																												//get modulo by 30 to indicate diodes position
	diode = (heading_degrees - modulo)/30;																										//get diode value from ring
	return diode;																																							//return this value
}

/*----------------------------------------------------------------------------
  Return angle from mag3110 indicate position of microcontroler.
 *---------------------------------------------------------------------------*/
int32_t return_angle_mag3110(int x_value, int y_value, int average_axis_x, int average_axis_y) {
	int32_t val_to_ret = 0;
	float degree = 0;	
	degree = (atan2((y_value-average_axis_y),(x_value-average_axis_x)));
	if (degree < 0)																																						//value correction to get values from 0 to 2*pi																																		
  {
    degree += 2 * PI;
  } 
  if (degree > 2 * PI)
  {
    degree -= 2 * PI;
  }
	degree = degree * (180/PI);
	degree = floor(degree);
	val_to_ret = degree;
	return val_to_ret;
}

/*----------------------------------------------------------------------------
  Return angle from mma8451q.
 *---------------------------------------------------------------------------*/
int32_t get_degree_mma8451q(int val_1, int val_2) {
	float degree = 0;
	int32_t val_to_ret = 0;
	degree = atan2(val_1,val_2);
	if (degree < 0)																																						//value correction to get values from 0 to 2*pi																																		
  {
    degree += 2 * PI;
  } 
  if (degree > 2 * PI)
  {
    degree -= 2 * PI;
  }
	degree = degree * 180/PI;
	degree = floor(degree);
	val_to_ret = degree;
	return val_to_ret;
}







