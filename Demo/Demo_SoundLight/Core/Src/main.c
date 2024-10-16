/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lora/radio/radio.h"
#include "lora/sx126x/sx126x_bsp.h"
#include "lora/sx126x/sx126x.h"
#include "bh1750/bh1750.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define RF_FREQUENCY 923000000  // Hz
#define TX_OUTPUT_POWER 22		// dBm
#define LORA_BANDWIDTH 0		// [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1		// [1: 4/5, 2: 4/6,  3: 4/7,  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define BUFFER_SIZE 64 // Define the payload size here

#define ALL_DEVEUI       0xFF
#define SENSOR_DEVEUI    0x74
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef enum {
	CMD_UNKNOWN,
	CMD_STATUS,
	CMD_STATUS_DAILY,
	CMD_SET_STANDBY_TIME,
	CMD_ACK,
	CMD_STOP_SENSOR,
	CMD_STATUS_ENDCALLPOINT,
} lora_pktcmd_t;

typedef struct {
	/// Receive flag
	uint8_t recv;
	/// Received signal param
	int16_t rssi;
	int8_t  snr;
	/// Received payload
	uint8_t recvbuffer[BUFFER_SIZE];
	uint8_t recvlen;
	/// Received parse param
	uint8_t deveui;
	lora_pktcmd_t cmd;
	uint8_t value_len;
	uint8_t value[8];
	uint8_t req_ack;
} lora_rxpkt_t;
typedef struct {
	uint8_t deveui;
	lora_pktcmd_t cmd;
	uint8_t value_len;
	uint8_t value[8];
	uint8_t req_ack;
	uint8_t resp_ack;
} lora_txpkt_t;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void OnTxDone(void);
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static RadioEvents_t RadioEvents;

volatile uint8_t start_process = 0;
lora_rxpkt_t rxpkt = {
	.recv = 0,
	.rssi = 0,
	.snr  = 0,
	.recvlen = 0,
	.deveui = 0,
	.cmd = CMD_UNKNOWN,
	.value_len = 0,
	.req_ack = 0,
};
lora_txpkt_t txpkt = {
	.deveui = 0,
	.cmd = CMD_UNKNOWN,
	.value_len = 0,
	.req_ack = 0,
	.resp_ack = 1,
};
uint32_t ack_timeout = 0;
uint32_t standby_time = 0;
uint32_t tick = 0;
uint8_t start_measure = 0;
float light_lux = 0;
uint32_t sound_decibel = 0;
uint32_t batt_volt = 0;

void send_packet(void);
uint32_t adc_get_value(uint32_t channel);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
//  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  BH1750_Init(&hi2c1);
  BH1750_SetMode(CONTINUOUS_HIGH_RES_MODE_2);

	SX126xIoInit();
	uint16_t readSyncWord = 0;
	SX126xReadRegisters(REG_LR_SYNCWORD, (uint8_t *)&readSyncWord, 2);


	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	Radio.Init(&RadioEvents);
	Radio.SetChannel(RF_FREQUENCY);
	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
					  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
					  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
					  true, 0, 0, LORA_IQ_INVERSION_ON, 0);
	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
					  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
					  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
					  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

	Radio.Rx(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Radio.BgIrqProcess();

	  if(start_process == 1){
		  if(HAL_GetTick() - tick > standby_time){
			  tick = HAL_GetTick();
			  start_measure = 1;
		  }
	  }

	  if(start_measure == 1 && start_process == 1){
		  if(HAL_GetTick() - tick > 1000){
			  tick = HAL_GetTick();

			  BH1750_ReadLight(&light_lux);
			  sound_decibel = adc_get_value(ADC_CHANNEL_6);
			  batt_volt = adc_get_value(ADC_CHANNEL_5);

			  txpkt.deveui = SENSOR_DEVEUI;
			  txpkt.cmd = CMD_STATUS;
			  txpkt.value_len = 3;
			  txpkt.value[0] = batt_volt;
			  txpkt.value[1] = sound_decibel;
			  txpkt.value[2] = (uint8_t)(light_lux*10.0);
			  txpkt.req_ack = 0;
			  send_packet();
		  }
	  }

	  /// Check ACK timeout
	  if(txpkt.req_ack == 1 && txpkt.resp_ack == 0){
		  if(HAL_GetTick() - ack_timeout > 5000){
			  txpkt.req_ack = 0;
		  }
	  }

	  if(rxpkt.recv == 1){
		  if(rxpkt.deveui == SENSOR_DEVEUI || rxpkt.deveui == ALL_DEVEUI){
			  switch(rxpkt.cmd){
				  case CMD_ACK:
			  		  txpkt.req_ack = 0;
			  		  txpkt.resp_ack = 1;
				  break;
				  case CMD_SET_STANDBY_TIME:
					  standby_time = rxpkt.value[0]*1000;
					  start_process = 1;
					  tick = HAL_GetTick();
				  break;
				  case CMD_STOP_SENSOR:
					  start_process = 0;
					  start_measure = 0;
				  break;
				  default:
				  break;
			  }

			  /// Packet require ACK
			  if(rxpkt.req_ack == 1){
				  txpkt.deveui = rxpkt.deveui;
				  txpkt.cmd = CMD_ACK;
				  txpkt.value_len = 0;
				  txpkt.req_ack = 0;
				  send_packet();
			  }
		  }

		  rxpkt.recv = 0;
		  rxpkt.recvlen = 0;
		  rxpkt.deveui = 0;
		  rxpkt.cmd = CMD_UNKNOWN;
		  rxpkt.value_len = 0;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == LORA_INT_Pin){
		extern void RadioOnDioIrq(void);
		RadioOnDioIrq();
	}
}

uint32_t adc_get_value(uint32_t channel){
	uint32_t ADCxConvertedValues = 0;
	ADC_ChannelConfTypeDef sConfig = {0};

	MX_ADC1_Init();
	if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
		Error_Handler();

	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		Error_Handler();

	if (HAL_ADC_Start(&hadc1) != HAL_OK)
		Error_Handler();
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	HAL_ADC_Stop(&hadc1);
	ADCxConvertedValues = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_DeInit(&hadc1);

	return ADCxConvertedValues;
}





void send_packet(void){
	uint8_t txbuffer[BUFFER_SIZE];

	txpkt.resp_ack = 0;
	txbuffer[0] = txpkt.deveui;
	txbuffer[1] = (uint8_t)txpkt.cmd;
	txbuffer[2] = txpkt.req_ack;
	txbuffer[3] = txpkt.value_len;
	for(uint8_t i=0; i<txpkt.value_len; i++)
		txbuffer[4+i] = txpkt.value[i];

	if(txpkt.req_ack == 1) ack_timeout = HAL_GetTick();

	Radio.Send(txbuffer, 4+txpkt.value_len);
}

void OnTxDone(void){
	Radio.Rx(0);
}

/**@brief Function to be executed on Radio Rx Done event
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	rxpkt.recv = 1;

	rxpkt.rssi = rssi;
	rxpkt.snr = snr;

	rxpkt.recvlen = size;
	memset(rxpkt.recvbuffer, 0, BUFFER_SIZE);
	memcpy(rxpkt.recvbuffer, payload, rxpkt.recvlen);

	rxpkt.deveui = rxpkt.recvbuffer[0];
	rxpkt.cmd = (lora_pktcmd_t)rxpkt.recvbuffer[1];
	rxpkt.req_ack = rxpkt.recvbuffer[2];
	rxpkt.value_len = rxpkt.recvbuffer[3];
	for(uint8_t i=0; i<rxpkt.value_len; i++)
		rxpkt.value[i] = rxpkt.recvbuffer[i+4];
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
