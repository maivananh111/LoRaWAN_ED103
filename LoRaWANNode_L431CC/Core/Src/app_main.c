/*
 * app_main.c
 *
 *  Created on: Jul 17, 2024
 *      Author: anh
 */

#include "main.h"
#include "lptim.h"
#include "spi.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Tracelog/TraceLog.h"
#include "LoRaWAN/port.h"
#include "LoRaWAN/phy/radio.h"
#include "LoRaWAN/mac/LoRaMacHelper.h"
#include "string.h"

static const char *TAG = "LORAWAN";

#define SCHED_MAX_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE 60										  /**< Maximum number of events in the scheduler queue. */
#define LORAWAN_APP_DATA_BUFF_SIZE 64  /**< Size of the data to be transmitted. */
#define LORAWAN_APP_TX_DUTYCYCLE 10000 /**< Defines the application data transmission duty cycle. 10s, value in [ms]. */
#define APP_TX_DUTYCYCLE_RND 1000	   /**< Defines a random delay for application data transmission duty cycle. 1s, value in [ms]. */
#define JOINREQ_NBTRIALS 3			   /**< Number of trials for the join request. */

uint8_t nodeDeviceEUI[8] = { 0x00, 0x95, 0x64, 0x1F, 0xDA, 0x91, 0x19, 0x0B };
uint8_t nodeAppEUI[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x01, 0xE1 };
uint8_t nodeAppKey[16] = { 0x07, 0xC0, 0x82, 0x0C, 0x30, 0xB9, 0x08, 0x70, 0x0C, 0x0F, 0x70, 0x06, 0x00, 0xB0, 0xBE, 0x09 };
//uint32_t nodeDevAddr = 0x260116F8;
//uint8_t nodeNwsKey[16] = { 0x7E, 0xAC, 0xE2, 0x55, 0xB8, 0xA5, 0xE2, 0x69, 0x91,
//		0x51, 0x96, 0x06, 0x47, 0x56, 0x9D, 0x23 };
//uint8_t nodeAppsKey[16] = { 0xFB, 0xAC, 0xB6, 0x47, 0xF3, 0x58, 0x45, 0xC7,
//		0x50, 0x7D, 0xBF, 0x16, 0x8B, 0xA8, 0xC1, 0x7C };

static void lorawan_has_joined_handler(void);
static void lorawan_rx_handler(lmh_app_data_t *app_data);
static void lorawan_confirm_class_handler(DeviceClass_t Class);
static void lorawan_join_failed_handler(void);
static void send_lora_frame(void);
static uint32_t timers_init(void);

TimerEvent_t appTimer;						///< LoRa tranfer timer instance.
static uint8_t m_lora_app_data_buffer[LORAWAN_APP_DATA_BUFF_SIZE];///< Lora user application data buffer.
static lmh_app_data_t m_lora_app_data = { m_lora_app_data_buffer, 0, 0, 0, 0 }; ///< Lora user application data structure.
static lmh_param_t lora_param_init = { LORAWAN_ADR_ON, LORAWAN_DEFAULT_DATARATE,
		LORAWAN_PUBLIC_NETWORK, JOINREQ_NBTRIALS, LORAWAN_DEFAULT_TX_POWER,
		LORAWAN_DUTYCYCLE_OFF };
static lmh_callback_t lora_callbacks = { BoardGetBatteryLevel, BoardGetUniqueId,
		BoardGetRandomSeed, lorawan_rx_handler, lorawan_has_joined_handler,
		lorawan_confirm_class_handler, lorawan_join_failed_handler };

void app_main(void) {
	MX_SPI1_Init();
	MX_LPTIM1_Init();

	Radio_HardWare_Init();

	timers_init();

	lmh_setDevEui(nodeDeviceEUI);
	lmh_setAppEui(nodeAppEUI);
	lmh_setAppKey(nodeAppKey);
//	lmh_setNwkSKey(nodeNwsKey);
//	lmh_setAppSKey(nodeAppsKey);
//	lmh_setDevAddr(nodeDevAddr);



	uint32_t err_code = lmh_init(&lora_callbacks, lora_param_init, true, CLASS_A, LORAMAC_REGION_AS923_2, false);
	if (err_code != 0)
		LOG_ERROR(TAG, "lmh_init failed - %d\n", err_code);
	if (!lmh_setSubBandChannels(1))
		LOG_ERROR(TAG, "lmh_setSubBandChannels failed. Wrong sub band requested?");

	lmh_join();

	while (1) {
		vTaskDelay(1000);
	}
}

static void lorawan_join_failed_handler(void) {
	LOG_ERROR(TAG, "OVER_THE_AIR_ACTIVATION failed!");
}

static void lorawan_has_joined_handler(void) {
	LOG_INFO(TAG, "OVER_THE_AIR_ACTIVATION OKE, JOINED");
	lmh_class_request(CLASS_A);

	TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
	TimerStart(&appTimer);
}

static void lorawan_rx_handler(lmh_app_data_t *app_data) {
	LOG_INFO(TAG, "LoRa Packet received on port %d, size:%d, rssi:%d, snr:%d\n",
			app_data->port, app_data->buffsize, app_data->rssi, app_data->snr);

	switch (app_data->port) {
		case 3:
			// Port 3 switches the class
			if (app_data->buffsize == 1) {
				switch (app_data->buffer[0]) {
					case 0:
						lmh_class_request(CLASS_A);
					break;

					case 1:
						lmh_class_request(CLASS_B);
					break;

					case 2:
						lmh_class_request(CLASS_C);
					break;

					default:
					break;
				}
			}
		break;

		case LORAWAN_APP_PORT:
			// YOUR_JOB: Take action on received data
		break;

		default:
		break;
	}
}

static void lorawan_confirm_class_handler(DeviceClass_t Class) {
	LOG_INFO(TAG, "switch to class %c done\n", "ABC"[Class]);

	m_lora_app_data.buffsize = 0;
	m_lora_app_data.port = LORAWAN_APP_PORT;
	lmh_send(&m_lora_app_data, LMH_UNCONFIRMED_MSG);
}

static void send_lora_frame(void) {
	if (lmh_join_status_get() != LMH_SET) {
		//Not joined, try again later
		LOG_ERROR(TAG, "Did not join network, skip sending frame");
		return;
	}

	uint32_t i = 0;
	m_lora_app_data.port = LORAWAN_APP_PORT;
	m_lora_app_data.buffer[i++] = 'H';
	m_lora_app_data.buffer[i++] = 'e';
	m_lora_app_data.buffer[i++] = 'l';
	m_lora_app_data.buffer[i++] = 'l';
	m_lora_app_data.buffer[i++] = 'o';
	m_lora_app_data.buffer[i++] = ' ';
	m_lora_app_data.buffer[i++] = 'w';
	m_lora_app_data.buffer[i++] = 'o';
	m_lora_app_data.buffer[i++] = 'r';
	m_lora_app_data.buffer[i++] = 'l';
	m_lora_app_data.buffer[i++] = 'd';
	m_lora_app_data.buffer[i++] = '!';
	m_lora_app_data.buffsize = i;

	lmh_error_status error = lmh_send(&m_lora_app_data, LMH_UNCONFIRMED_MSG);
	LOG_INFO(TAG, "lmh_send result %d\n", error);
}

static void tx_lora_periodic_handler(void *arg) {
	TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE);
	TimerStart(&appTimer);
	LOG_INFO(TAG, "Sending frame");
	send_lora_frame();
}

static uint32_t timers_init(void) {
	TimerInit(&appTimer, tx_lora_periodic_handler);
	return 0;
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

