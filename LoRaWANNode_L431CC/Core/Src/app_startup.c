
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

#include "string.h"
#include "Tracelog/TraceLog.h"


void SystemClock_Config(void);


static void main_task(void *arg){
	extern void app_main(void);
	app_main();
	vTaskDelete(NULL);
}

static void tracelog(char *str){
	HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 1000);
}


void sys_main(void){
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	log_monitor_init(tracelog);

	LOG_INFO("STARTUP", "App startup.");

	osKernelInitialize();
	if(xTaskCreate(main_task, "main_task", 512, NULL, 1, NULL) != pdTRUE)
		LOG_ERROR("STARTUP", "sys_main task startup fail");
	osKernelStart();
}

