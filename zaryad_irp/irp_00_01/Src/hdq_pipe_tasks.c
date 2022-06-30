/*
 * hdq_pipe_tasks.c
 *
 *  Created on: Jun 21, 2022
 *      Author: agant
 */

#include "hdq_pipe.h"
#include "gpio.h"

extern uint64_t usec10tick;

void send_brake()
{
	// drop bat_data_out_Pin
	GPIOB->BRR = bat_data_out_Pin;
	uint64_t usec10tick_frozen = usec10tick;
	// wait 200 usec
	while((usec10tick - usec10tick_frozen) < 30){};
	// rise bat_data_out_Pin
	GPIOB->BSRR = bat_data_out_Pin;
}

void send_restore()
{
	// rise bat_data_out_Pin
	GPIOB->BSRR = bat_data_out_Pin;
	uint64_t usec10tick_frozen = usec10tick;
	// wait 100 usec
	while((usec10tick - usec10tick_frozen) < 10){};
	// drop bat_data_out_Pin
	GPIOB->BRR = bat_data_out_Pin;

}

#define SEND_1 \
		while((GPIOB->IDR & bat_data_in_Pin) != (uint32_t)GPIO_PIN_RESET){}; \
		GPIOB->BRR = bat_data_out_Pin; \
		uint64_t usec10tick_frozen = usec10tick; \
		while((usec10tick - usec10tick_frozen) < 3){}; \
		GPIOB->BSRR = bat_data_out_Pin; \
		while((usec10tick - usec10tick_frozen) < 20){}

#define SEND_0 \
		while((GPIOB->IDR & bat_data_in_Pin) != (uint32_t)GPIO_PIN_RESET){}; \
		GPIOB->BRR = bat_data_out_Pin; \
		uint64_t usec10tick_frozen = usec10tick; \
		while((usec10tick - usec10tick_frozen) < 13){}; \
		GPIOB->BSRR = bat_data_out_Pin; \
		while((usec10tick - usec10tick_frozen) < 20){}


void send_byte(uint8_t data)
{
	uint8_t aux = data;

	for(int i=0; i<8; i++)
	{
		if((aux & (uint8_t)1) == 0)
		{
			GPIOB->BRR = bat_data_out_Pin;
			uint64_t usec10tick_frozen = usec10tick;
			while((usec10tick - usec10tick_frozen) < 13){};
			GPIOB->BSRR = bat_data_out_Pin;
			if(i<7)
				while((usec10tick - usec10tick_frozen) < 20){};
		}
		else
		{
			GPIOB->BRR = bat_data_out_Pin;
			uint64_t usec10tick_frozen = usec10tick;
			while((usec10tick - usec10tick_frozen) < 3){};
			GPIOB->BSRR = bat_data_out_Pin;
			if(i<7)
				while((usec10tick - usec10tick_frozen) < 20){};
		}

		aux =  aux >> 1;
	}

}




