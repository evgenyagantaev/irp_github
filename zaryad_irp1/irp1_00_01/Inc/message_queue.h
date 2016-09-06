/*
 * message_queue.h
 *
 *  Created on: Aug 26, 2016
 *      Author: root
 */

#ifndef INC_MESSAGE_QUEUE_H_
#define INC_MESSAGE_QUEUE_H_

#include "stdint.h"

#define MSG_GET_BATT_VOLTAGE 0x21
#define MSG_GET_BATT_CURRENT 0x22
#define MSG_GET_BATT_TEMPERATURE 0x23

#define COMMAND_CHARGE_ON		0x25
#define COMMAND_CHARGE_OFF		0x26
#define COMMAND_DISCHARGE_ON	0x27
#define COMMAND_DISCHARGE_OFF	0x28

static uint8_t message_queue[64];

uint8_t message_push(uint8_t message);
uint8_t message_pop();

#endif /* INC_MESSAGE_QUEUE_H_ */
