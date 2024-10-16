/*
 * port.c
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#include "port.h"

uint32_t BoardGetRandomSeed(void){
	// return random(255);
	return 128;
}

void BoardGetUniqueId(uint8_t *id) {
	uint8_t name[32] = {0};

	id[7] = name[7] + name[15] + name[23] + name[31];
	id[6] = name[6] + name[14] + name[22] + name[30];
	id[5] = name[5] + name[13] + name[21] + name[29];
	id[4] = name[4] + name[12] + name[20] + name[28];
	id[3] = name[3] + name[11] + name[19] + name[27];
	id[2] = name[2] + name[10] + name[18] + name[26];
	id[1] = name[1] + name[9] + name[17] + name[25];
	id[0] = name[0] + name[8] + name[16] + name[24];
}

uint8_t BoardGetBatteryLevel(void) {
	uint8_t batteryLevel = 50;

	return batteryLevel;
}

void BoardDisableIrq(void) {
	__disable_irq();

}

void BoardEnableIrq(void) {
	__enable_irq();
}
