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
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "radio.h"
#include "Commissioning.h"
#include "LmHandler.h"
#include "LmhpCompliance.h"
#include "utilities.h"
#include "logger.h"
#include "timer_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define TAG "WAN"
void log_port_flush_string(char *string, uint16_t length){
	HAL_UART_Transmit(&huart1, (uint8_t *)string, length, 1000);
}

uint32_t log_port_get_systime(void){
	return (uint32_t)HAL_GetTick();
}

#define LORAWAN_DEFAULT_CLASS                       CLASS_A
#define APP_TX_DUTYCYCLE                            20000
#define APP_TX_DUTYCYCLE_RND                        1000
#define LORAWAN_ADR_STATE                           LORAMAC_HANDLER_ADR_ON
#define LORAWAN_DEFAULT_DATARATE                    DR_0
#define LORAWAN_DEFAULT_CONFIRMED_MSG_STATE         LORAMAC_HANDLER_UNCONFIRMED_MSG
#define LORAWAN_APP_DATA_BUFFER_MAX_SIZE            242
#define LORAWAN_DUTYCYCLE_ON						true
#define LORAWAN_APP_PORT                            2

typedef enum {
	LORAMAC_HANDLER_TX_ON_TIMER,
	LORAMAC_HANDLER_TX_ON_EVENT,
} LmHandlerTxEvents_t;


static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE] = "Hello";
static LmHandlerAppData_t AppData = {
	.Buffer = AppDataBuffer,
	.BufferSize = 5,
	.Port = 0
};

static TimerEvent_t TxTimer;

static void OnMacProcessNotify(void);
static void OnNvmContextChange(LmHandlerNvmContextStates_t state);
static void OnNetworkParametersChange(CommissioningParams_t *params);
static void OnMacMcpsRequest(LoRaMacStatus_t status, McpsReq_t *mcpsReq);
static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq);
static void OnJoinRequest(LmHandlerJoinParams_t *params);
static void OnTxData(LmHandlerTxParams_t *params);
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);
static void OnClassChange(DeviceClass_t deviceClass);
static void OnBeaconStatusChange(LoRaMAcHandlerBeaconParams_t *params);

static void PrepareTxFrame(void);
static void StartTxProcess(LmHandlerTxEvents_t txEvent);
static void UplinkProcess(void);
static void OnTxTimerEvent(void *context);

uint32_t BoardGetRandomSeed(void);
void BoardGetUniqueId(uint8_t *id);
uint8_t BoardGetBatteryLevel(void);

static LmHandlerCallbacks_t LmHandlerCallbacks ={
    .GetBatteryLevel = BoardGetBatteryLevel,
    .GetTemperature = NULL,
    .GetUniqueId = BoardGetUniqueId,
    .GetRandomSeed = BoardGetRandomSeed,
    .OnMacProcess = OnMacProcessNotify,
    .OnNvmContextChange = OnNvmContextChange,
    .OnNetworkParametersChange = OnNetworkParametersChange,
    .OnMacMcpsRequest = OnMacMcpsRequest,
    .OnMacMlmeRequest = OnMacMlmeRequest,
    .OnJoinRequest = OnJoinRequest,
    .OnTxData = OnTxData,
    .OnRxData = OnRxData,
    .OnClassChange= OnClassChange,
    .OnBeaconStatusChange = OnBeaconStatusChange
};

static LmHandlerParams_t LmHandlerParams ={
    .Region = ACTIVE_REGION,
    .AdrEnable = LORAWAN_ADR_STATE,
    .TxDatarate = LORAWAN_DEFAULT_DATARATE,
    .PublicNetworkEnable = LORAWAN_PUBLIC_NETWORK,
    .DutyCycleEnabled = LORAWAN_DUTYCYCLE_ON,
    .DataBufferMaxSize = LORAWAN_APP_DATA_BUFFER_MAX_SIZE,
    .DataBuffer = AppDataBuffer
};

static LmhpComplianceParams_t LmhpComplianceParams ={
    .AdrEnabled = LORAWAN_ADR_STATE,
    .DutyCycleEnabled = LORAWAN_DUTYCYCLE_ON,
    .StopPeripherals = NULL,
    .StartPeripherals = NULL,
};


static volatile uint8_t IsMacProcessPending = 0;
static volatile uint8_t IsTxFramePending = 0;
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
  MX_ADC1_Init();
//  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  TIMER_IF_Init();
  LOGI(TAG, "cc");
  Radio_HardWare_Init();
  LmHandlerInit( &LmHandlerCallbacks, &LmHandlerParams );
  LmHandlerPackageRegister( PACKAGE_ID_COMPLIANCE, &LmhpComplianceParams );
  LmHandlerJoin( );
  LOGI(TAG, "JOINING");
  StartTxProcess( LORAMAC_HANDLER_TX_ON_TIMER );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		LmHandlerProcess( );
		UplinkProcess( );

//      CRITICAL_SECTION_BEGIN( );
		if( IsMacProcessPending == 1 ){
			IsMacProcessPending = 0;

		}
		else{
		  HAL_Delay(10);
		}
//      CRITICAL_SECTION_END( );
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//	if (GPIO_Pin == LORA_INT_Pin) {
		RadioOnDioIrq();
//	}
}


static void OnMacProcessNotify(void) {
	IsMacProcessPending = 1;
}

static void OnNvmContextChange(LmHandlerNvmContextStates_t state) {
	LOGI(TAG, "OnNvmContextChange");
}

static void OnNetworkParametersChange(CommissioningParams_t *params) {
	LOGI(TAG, "OnNetworkParametersChange");
}

static void OnMacMcpsRequest(LoRaMacStatus_t status, McpsReq_t *mcpsReq) {
	LOGI(TAG, "OnMacMcpsRequest");
}

static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq) {
	LOGI(TAG, "OnMacMlmeRequest, status = %d", status);
}

static void OnJoinRequest(LmHandlerJoinParams_t *params) {
	LOGI(TAG, "OnJoinRequest");

	if (params->Status == LORAMAC_HANDLER_ERROR) {
		LOGE(TAG, "JOIN FAILED");
		LmHandlerJoin();
	}
	else {
		LOGI(TAG, "JOINED");
		LmHandlerRequestClass( LORAWAN_DEFAULT_CLASS);
	}
}

static void OnTxData(LmHandlerTxParams_t *params) {
	LOGI(TAG, "OnTxData");
}

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	switch (appData->Port) {
		case 1:
		case LORAWAN_APP_PORT:
		break;
		default:
		break;
	}
	LOGI(TAG, "OnRxData");
}

static void OnClassChange(DeviceClass_t deviceClass) {
	LmHandlerAppData_t appData = { .Buffer = NULL, .BufferSize = 0, .Port = 0 };
	LmHandlerSend(&appData, LORAMAC_HANDLER_UNCONFIRMED_MSG);
	LOGI(TAG, "OnClassChange");
}

static void OnBeaconStatusChange(LoRaMAcHandlerBeaconParams_t *params) {
	switch (params->State) {
		case LORAMAC_HANDLER_BEACON_RX: {
			break;
		}
		case LORAMAC_HANDLER_BEACON_LOST:
		case LORAMAC_HANDLER_BEACON_NRX: {
			break;
		}
		default: {
			break;
		}
	}
	LOGI(TAG, "OnBeaconStatusChange");
}

/*!
 * Prepares the payload of the frame and transmits it.
 */
static void PrepareTxFrame(void) {
	if (LmHandlerIsBusy() == true)
		return;

	AppData.Port = LORAWAN_APP_PORT;
	if (LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE)
			== LORAMAC_HANDLER_SUCCESS) {
		LOGI(TAG, "SEND uplink done");
	}
	else{
		LOGE(TAG, "SEND uplink failed");
	}
}

static void StartTxProcess(LmHandlerTxEvents_t txEvent) {
	switch (txEvent) {
		default:
			// Intentional fall through
		case LORAMAC_HANDLER_TX_ON_TIMER: {
			// Schedule 1st packet transmission
			TimerInit(&TxTimer, OnTxTimerEvent);
			TimerSetValue(&TxTimer, APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND ));
			OnTxTimerEvent( NULL);
		}
		break;
		case LORAMAC_HANDLER_TX_ON_EVENT: {
		}
		break;
	}
}

static void UplinkProcess(void) {
	uint8_t isPending = 0;
	CRITICAL_SECTION_BEGIN();
	isPending = IsTxFramePending;
	IsTxFramePending = 0;
	CRITICAL_SECTION_END();
	if (isPending == 1)
		PrepareTxFrame();
}

/*!
 * Function executed on TxTimer event
 */
static void OnTxTimerEvent(void *context) {
	TimerStop(&TxTimer);
	IsTxFramePending = 1;
	// Schedule next transmission
	TimerSetValue(&TxTimer, APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND ));
	TimerStart(&TxTimer);
}



uint32_t BoardGetRandomSeed(void) {
	return 128;
}

void BoardGetUniqueId(uint8_t *id) {
	id[7] = ((*(uint32_t*) UID_BASE+1)) >> 24;
	id[6] = ((*(uint32_t*) UID_BASE+1)) >> 16;
	id[5] = ((*(uint32_t*) UID_BASE+1)) >> 8;
	id[4] = ((*(uint32_t*) UID_BASE+1));
	id[3] = ((*(uint32_t*) UID_BASE)) >> 24;
	id[2] = ((*(uint32_t*) UID_BASE)) >> 16;
	id[1] = ((*(uint32_t*) UID_BASE)) >> 8;
	id[0] = ((*(uint32_t*) UID_BASE));
}

uint8_t BoardGetBatteryLevel( void ){
	return 127;
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
