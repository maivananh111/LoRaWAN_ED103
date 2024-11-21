/*
 * app_main.c
 *
 *  Created on: Jul 17, 2024
 *      Author: anh
 */



#include "main.h"
#include "spi.h"

#include "FreeRTOS.h"
#include "task.h"

#include "rtc-board.h"
#include "logger.h"
#include "radio.h"
#include "string.h"
#include "lora_apps.h"

//static const char *TAG = "LORAWAN";


/* AS923_2 */

void app_main(void) {
	RtcInit();
	Radio_HardWare_Init();
	LoRaWAN_Init();

	vTaskDelete(NULL);
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == LORA_INT_Pin) {
		RadioOnDioIrq();
	}
}

