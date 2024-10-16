
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "adc.h"
#include "spi.h"
#include "rtc.h"

#include "stdio.h"
#include "string.h"
#include "logger.h"


void SystemClock_Config(void);


static void main_task(void *arg){
	extern void app_main(void);
	app_main();
	vTaskDelete(NULL);
}

void log_port_flush_string(char *string, uint16_t length){
	HAL_UART_Transmit(&huart1, (uint8_t *)string, length, 1000);
}

uint32_t log_port_get_systime(void){
	return (uint32_t)HAL_GetTick();
}

int _write(int file, char *ptr, int len) {
	(void) file;

	HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, 1000);
	fflush(stdout);

	return len;
}


void sys_main(void) {
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_ADC1_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();

	LOGI("STARTUP", "App startup.");

	osKernelInitialize();
	if (xTaskCreate(main_task, "main_task", 1024, NULL, 1, NULL) != pdTRUE)
		LOGI("STARTUP", "sys_main task startup fail");
	osKernelStart();
}

