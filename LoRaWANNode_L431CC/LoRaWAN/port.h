/*
 * port.h
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#ifndef LORAWAN_PORT_H_
#define LORAWAN_PORT_H_

#include "stm32l4xx_hal.h"
#include "spi.h"




#define RADIO_PHY_HSPI hspi1


void BoardDisableIrq(void);

void BoardEnableIrq(void);

#endif /* LORAWAN_PORT_H_ */
