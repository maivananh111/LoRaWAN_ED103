

#include <lora_apps.h>
#include "platform.h"
#include "sys_app.h"
#include "cmsis_os.h"
#include "stm32_timer.h"
#include "app_version.h"
#include "lorawan_version.h"
#include "lora_info.h"
#include "LmHandler.h"
#include "adc_if.h"
#include "CayenneLpp.h"
#include "flash_if.h"


#include "stdlib.h"
#include "string.h"
#include "logger.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"



/* USER CODE BEGIN PTD */
static const char *TAG = "WAN";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

UTIL_TIMER_Time_t TxPeriodicity = APP_TX_DUTYCYCLE;
/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void SendTxData(void);
static void OnJoinRequest(LmHandlerJoinParams_t *joinParams);
static void OnTxData(LmHandlerTxParams_t *params);
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);
static void OnBeaconStatusChange(LmHandlerBeaconParams_t *params);
static void OnSysTimeUpdate(void);
static void OnClassChange(DeviceClass_t deviceClass);
static void OnMacProcessNotify(void);
static void OnTxPeriodicityChanged(uint32_t periodicity);
static void OnTxFrameCtrlChanged(LmHandlerMsgTypes_t isTxConfirmed);
static void OnPingSlotPeriodicityChanged(uint8_t pingSlotPeriodicity);
static void OnSystemReset(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static ActivationType_t ActivationType = LORAWAN_DEFAULT_ACTIVATION_TYPE;
static bool ForceRejoin = LORAWAN_FORCE_REJOIN_AT_BOOT;
static LmHandlerCallbacks_t LmHandlerCallbacks ={
	.GetBatteryLevel =              GetBatteryLevel,
	.GetTemperature =               GetTemperatureLevel,
	.GetUniqueId =                  GetUniqueId,
	.GetDevAddr =                   GetDevAddr,
	.OnRestoreContextRequest =      NULL,
	.OnStoreContextRequest =        NULL,
	.OnMacProcess =                 OnMacProcessNotify,
	.OnNvmDataChange =              NULL,
	.OnJoinRequest =                OnJoinRequest,
	.OnTxData =                     OnTxData,
	.OnRxData =                     OnRxData,
	.OnClassChange =                OnClassChange,
	.OnBeaconStatusChange =         OnBeaconStatusChange,
	.OnSysTimeUpdate =              OnSysTimeUpdate,
	.OnTxPeriodicityChanged =       OnTxPeriodicityChanged,
	.OnTxFrameCtrlChanged =         OnTxFrameCtrlChanged,
	.OnPingSlotPeriodicityChanged = OnPingSlotPeriodicityChanged,
	.OnSystemReset =                OnSystemReset,
};
LmHandlerParams_t LmHandlerParams ={
	.ActiveRegion =             ACTIVE_REGION,
	.DefaultClass =             LORAWAN_DEFAULT_CLASS,
	.AdrEnable =                LORAWAN_ADR_STATE,
	.IsTxConfirmed =            LORAWAN_DEFAULT_CONFIRMED_MSG_STATE,
	.TxDatarate =               LORAWAN_DEFAULT_DATA_RATE,
	.TxPower =                  LORAWAN_DEFAULT_TX_POWER,
	.PingSlotPeriodicity =      LORAWAN_DEFAULT_PING_SLOT_PERIODICITY,
	.RxBCTimeout =              LORAWAN_DEFAULT_CLASS_B_C_RESP_TIMEOUT
};

static void Thd_LoraSendProcess(void *argument);
TaskHandle_t htask_lmhandler;
static void Thd_LmHandlerProcess(void *argument);




void LoRaWAN_Init(void){
	if (xTaskCreate(Thd_LmHandlerProcess, "Thd_LmHandlerProcess",
			CFG_LM_HANDLER_PROCESS_STACK_SIZE/4, NULL, CFG_LM_HANDLER_PROCESS_PRIORITY, &htask_lmhandler) != pdTRUE)
		Error_Handler();

	if (xTaskCreate(Thd_LoraSendProcess, "Thd_LoraSendProcess",
			CFG_APP_LORA_PROCESS_STACK_SIZE/4, NULL, CFG_APP_LORA_PROCESS_PRIORITY, NULL) != pdTRUE)
		Error_Handler();

	LoraInfo_Init();
	LmHandlerInit(&LmHandlerCallbacks, APP_VERSION);
	LmHandlerConfigure(&LmHandlerParams);

	LmHandlerJoin(ActivationType, ForceRejoin);
	LOGI(TAG, "JOINING");
}

static void Thd_LmHandlerProcess(void *argument){
	UNUSED(argument);
	uint32_t notify_val;
	while(1){
		xTaskNotifyWait(0, 0, &notify_val, portMAX_DELAY);
		LmHandlerProcess();
		(void)notify_val;
	}
}

static void Thd_LoraSendProcess(void *argument){
	UNUSED(argument);
	while(1){
		SendTxData();
		vTaskDelay(APP_TX_DUTYCYCLE);
	}
}



static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params){
	if(appData->BufferSize > 0)
		LOGW(TAG, "OnRxData: %s, RSSI: %d", appData->Buffer, params->Rssi);
}

static void SendTxData(void){
	LmHandlerErrorStatus_t status = LORAMAC_HANDLER_ERROR;
	UTIL_TIMER_Time_t nextTxIn = 0;
	LmHandlerAppData_t appdata;

	char json[6] = "HELLO";
	appdata.Port = LORAWAN_USER_APP_PORT;
	appdata.Buffer = (uint8_t *)json;
	appdata.BufferSize = 5;

	status = LmHandlerSend(&appdata, LmHandlerParams.IsTxConfirmed, true);
	if (status == LORAMAC_HANDLER_SUCCESS) {
		LOGI(TAG, "SENT UPLINK.");
	}
	else if (status == LORAMAC_HANDLER_DUTYCYCLE_RESTRICTED) {
       if(nextTxIn > 0)
           nextTxIn = LmHandlerGetDutyCycleWaitTime();
       LOGI(TAG, "Next Tx in %lums", nextTxIn);
	}
}

static void OnTxData(LmHandlerTxParams_t *params){
	if(params->Status == LORAMAC_EVENT_INFO_STATUS_OK){
		LOGI(TAG, "TX COMPLETE.");
	}
}


static void OnJoinRequest(LmHandlerJoinParams_t *joinParams){
	if(joinParams->Status == LORAMAC_HANDLER_SUCCESS){
		LOGI(TAG, "JOINED.");
	}
	else{
		LOGE(TAG, "JOIN ERROR, TRYING AGAIN.");
	}
}

static void OnMacProcessNotify(void){
	if(__get_IPSR() == 0U) xTaskNotify(htask_lmhandler, 1, eNoAction);
	else {
		BaseType_t yield;
		xTaskNotifyFromISR(htask_lmhandler, 1, eNoAction, &yield);
		if(yield) portEND_SWITCHING_ISR (yield);
	}
}

static void OnTxPeriodicityChanged(uint32_t periodicity){
	TxPeriodicity = periodicity;

	if (TxPeriodicity == 0){
		TxPeriodicity = APP_TX_DUTYCYCLE;
	}
}

static void OnTxFrameCtrlChanged(LmHandlerMsgTypes_t isTxConfirmed){
	LmHandlerParams.IsTxConfirmed = isTxConfirmed;
}

static void OnPingSlotPeriodicityChanged(uint8_t pingSlotPeriodicity){
	LmHandlerParams.PingSlotPeriodicity = pingSlotPeriodicity;
}

static void OnSystemReset(void){
	if ((LORAMAC_HANDLER_SUCCESS == LmHandlerHalt()) && (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET)){
		__NVIC_SystemReset();
	}
}


static void OnBeaconStatusChange(LmHandlerBeaconParams_t *params){}
static void OnSysTimeUpdate(void){}
static void OnClassChange(DeviceClass_t deviceClass){}





