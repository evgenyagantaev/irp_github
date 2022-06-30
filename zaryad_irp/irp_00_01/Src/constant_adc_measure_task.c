/*
 * int_adc_measure_task.c
 *
 *  Created on: Aug 24, 2016
 *      Author: root
 */

#include "constant_adc_measure_task.h"
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

#include "battery_control_task.h"

extern uint8_t addresses[];
extern uint16_t values[];
extern uint8_t battery_address;
extern uint16_t battery_value;
extern uint voltage;
extern int turn_off_display;
extern int D_VERSION;

extern int svd1_light;
extern int svd2_light;
extern int svd3_light;
extern int svd4_light;
extern int svd5_light;
extern int svd6_light;

extern int presentation_complete;

void constant_adc_measure_task()
{

	static uint32_t frozen_seconds_tick = 0;

	static int i = 0;
	static uint battery_voltage = 0;

	if (presentation_complete)
	{
		if(seconds_tick > frozen_seconds_tick +4)
		{
			frozen_seconds_tick = seconds_tick;

			battery_address = addresses[i];

			battery_control_task();
			uint aux;
			if(battery_value != 0xffff)
				aux = battery_value;
			else
				aux = 0;

			aux /= 100;
			values[i] = aux;
			battery_voltage += aux;

			if(i >= 8)
			{
				i = 0;
				voltage = battery_voltage;
				battery_voltage = 0;
			}


		}// end if(seconds_tick > frozen_seconds_tick)

	}// end if (presentation_complete)

}//void constant_adc_measure_task()
