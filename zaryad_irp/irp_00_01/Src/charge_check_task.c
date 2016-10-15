/*
 * charge_check_task.c
 *
 *  Created on: Oct 12, 2016
 *      Author: root
 */

#include "charge_check_task.h"
#include "spi_pipe.h"
#include "gpio.h"


static uint32_t frozen_seconds = 0;



void charge_check_task()
{

	uint32_t current_seconds = time_manager_get_seconds();

	if(current_seconds > frozen_seconds)
	{
		frozen_seconds = current_seconds;

		uint32_t charge_overall_level = 0;
	    uint32_t private_charge_level = 0;
	    uint16_t quote = 0;
                                                                                   
	    int i;
                                                                                   
	    for(i=0; i<4; i++)
	    {
	    	spi_pipe_send_command(COMMAND_GET_CHARGE_LEVEL, i);
	    	HAL_Delay(5);
	    	quote = spi_pipe_receive_data(&private_charge_level, i);
	    	if(quote == QUOTE_OK)
	    	{
	    		if((private_charge_level >= 0) && (private_charge_level <= 100))
	    			charge_overall_level += private_charge_level;
	    	}
	    }
                                                                                   
	    charge_overall_level /= 4;
                                                                                   
	    // debug *********
	    //charge_overall_level = 97;
                                                                                   
	    if(charge_overall_level >= 97)
	    {
	    	GPIOA->BRR = (charge100_led_green_out_Pin|charge75_led_green_out_Pin
	    			|charge50_led_green_out_Pin|charge25_led_green_out_Pin);
	    	GPIOA->BSRR = (charge100_led_green_out_Pin);
	    }
	    else if(charge_overall_level >= 75)
	    {
	    	GPIOA->BRR = (charge100_led_green_out_Pin|charge75_led_green_out_Pin
	    			|charge50_led_green_out_Pin|charge25_led_green_out_Pin);
	    	GPIOA->BSRR = (charge75_led_green_out_Pin);
	    }
	    else if(charge_overall_level >= 50)
	    {
	    	GPIOA->BRR = (charge100_led_green_out_Pin|charge75_led_green_out_Pin
	    			|charge50_led_green_out_Pin|charge25_led_green_out_Pin);
	    	GPIOA->BSRR = (charge50_led_green_out_Pin );
	    }
	    else if(charge_overall_level >= 5)
		{
			GPIOA->BRR = (charge100_led_green_out_Pin|charge75_led_green_out_Pin
					|charge50_led_green_out_Pin|charge25_led_green_out_Pin);
			GPIOA->BSRR = (charge25_led_green_out_Pin );
		}
	    else
	    {
	    	GPIOA->BRR = (charge100_led_green_out_Pin|charge75_led_green_out_Pin
	    			|charge50_led_green_out_Pin|charge25_led_green_out_Pin);

	    }
                                                                                   
	}

}