/*
 * spi1_transsiver.h
 *
 *  Created on: Aug 26, 2016
 *      Author: root
 */

#ifndef INC_SPI1_TRANSCEIVER_H_
#define INC_SPI1_TRANSCEIVER_H_

#include "stdint.h"

uint8_t spi1_read_message();
void spi1_send_data(uint32_t data);

#endif /* INC_SPI1_TRANSCEIVER_H_ */
