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
	while((usec10tick - usec10tick_frozen) < 20){};
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
			while((usec10tick - usec10tick_frozen) < 20){};
		}
		else
		{
			GPIOB->BRR = bat_data_out_Pin;
			uint64_t usec10tick_frozen = usec10tick;
			while((usec10tick - usec10tick_frozen) < 3){};
			GPIOB->BSRR = bat_data_out_Pin;
			while((usec10tick - usec10tick_frozen) < 20){};
		}

		aux =  aux >> 1;
	}

}

uint16_t receive_word()
{
	uint16_t aux = 1;
	uint16_t data = 0;

	for(int i=0; i<8; i++)
	{
		// wait for low level
		while((GPIOB->IDR & bat_data_in_Pin) != (uint32_t)GPIO_PIN_RESET){};
		uint64_t usec10tick_frozen = usec10tick;
		//wait for high level
		while((GPIOB->IDR & bat_data_in_Pin) == (uint32_t)GPIO_PIN_RESET){};
		uint64_t duration = usec10tick - usec10tick_frozen;

		if(duration <= 8)  // received 1
		{
			data += aux;
		}
		aux = aux << 1;
	}

	return data;
}





