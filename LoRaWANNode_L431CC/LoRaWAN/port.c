/*
 * port.c
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#include "port.h"



void BoardDisableIrq(void) {
	__disable_irq();

}

void BoardEnableIrq(void) {
	__enable_irq();
}
