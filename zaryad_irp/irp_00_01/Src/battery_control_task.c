/*
 * battery_control_task.c
 *
 *  Created on: Aug 24, 2016
 *      Author: root
 */

#include "battery_control_task.h"
#include "battery.h"
#include "ext_power_obj.h"
#include "time_management_task.h"
#include "spi_pipe.h"
#include "gpio.h"


//debug
#include "usart.h"
extern UART_HandleTypeDef huart1;
static char battery_control_message[128];



static uint32_t frozen_seconds;

void battery_control_task()
{

	//debug *********************
	//int32_t ext_pow_voltage = ext_pow_get_voltage();
	//sprintf(battery_control_message, "ext pow = %d\r\n", ext_pow_voltage);
	//HAL_UART_Transmit(&huart1, battery_control_message, strlen(battery_control_message), 500);

	// URGENT ACTIONS ****************************************************


	// check external power first
	int external_power_status = ext_pow_get_status();
	if(external_power_status != NORM) // something wrong
	{
		// check if we already turned everything off
		if(!emergency_turn_off_flag)
		{
			// turn off charge and discharge in all channels
			spi_pipe_send_command(COMMAND_CHARGE_OFF, 0);
			spi_pipe_send_command(COMMAND_CHARGE_OFF, 1);
			spi_pipe_send_command(COMMAND_CHARGE_OFF, 2);
			spi_pipe_send_command(COMMAND_CHARGE_OFF, 3);
			spi_pipe_send_command(COMMAND_DISCHARGE_OFF, 0);
			spi_pipe_send_command(COMMAND_DISCHARGE_OFF, 1);
			spi_pipe_send_command(COMMAND_DISCHARGE_OFF, 2);
			spi_pipe_send_command(COMMAND_DISCHARGE_OFF, 3);
			// change batteries status
			set_charge_flag(CHARGE_OFF, 0);
			set_charge_flag(CHARGE_OFF, 1);
			set_charge_flag(CHARGE_OFF, 2);
			set_charge_flag(CHARGE_OFF, 3);
			set_discharge_flag(DISCHARGE_OFF, 0);
			set_discharge_flag(DISCHARGE_OFF, 1);
			set_discharge_flag(DISCHARGE_OFF, 2);
			set_discharge_flag(DISCHARGE_OFF, 3);

			// turn off charge leds
			HAL_GPIO_WritePin(GPIOA, charge_led_red_out_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, chargeOK_led_green_out_Pin, GPIO_PIN_RESET);

			emergency_turn_off_flag = 1;

			//debug
			sprintf(battery_control_message, "turned off all charge and discharge\r\n");
			HAL_UART_Transmit(&huart1, battery_control_message, strlen(battery_control_message), 500);
		}
	}
	else // external power supply ok
	{
		emergency_turn_off_flag = 0;

		// NORMAL PERIODICAL ACTIONS *************************************************

		uint32_t current_seconds = time_manager_get_seconds();

		if(current_seconds > frozen_seconds)
		{
			frozen_seconds = current_seconds;

			//debug
			sprintf(battery_control_message, "seconds = %d\r\n", current_seconds);
			HAL_UART_Transmit(&huart1, battery_control_message, strlen(battery_control_message), 500);
			//debug *********************
			int32_t ext_pow_voltage = ext_pow_get_voltage();
			sprintf(battery_control_message, "ext pow = %d\r\n", ext_pow_voltage);
			HAL_UART_Transmit(&huart1, battery_control_message, strlen(battery_control_message), 500);

		}
	}


}
