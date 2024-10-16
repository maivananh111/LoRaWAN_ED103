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

#include "logger.h"
#include "radio.h"
#include "app_lorawan.h"
#include "string.h"

//static const char *TAG = "LORAWAN";




void app_main(void) {
	Radio_HardWare_Init();
	MX_LoRaWAN_Init();

	while (1) {


		vTaskDelay(1000);
	}

}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == LORA_INT_Pin) {
		RadioOnDioIrq();
	}
	if (GPIO_Pin == BTN_WAKE_Pin) {
		for (uint32_t i = 0; i < 120000; i++)
			__NOP(); //10ms
		if (HAL_GPIO_ReadPin(BTN_WAKE_GPIO_Port, BTN_WAKE_Pin)
				== GPIO_PIN_RESET) {

		}
	}
}

