/*
 * lora_porting.h
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#ifndef PORTING_LORA_PORTING_H_
#define PORTING_LORA_PORTING_H_

#ifdef __cplusplus
extern "C"{
#endif


#include "stm32l4xx_hal.h"



uint8_t GetBatteryLevel(void);
uint32_t GetRandomSeed(void);
void EnterSleepMode(uint32_t time);

#ifdef __cplusplus
}
#endif
#endif /* PORTING_LORA_PORTING_H_ */
