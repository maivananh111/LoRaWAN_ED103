

#include <lora_apps.h>
#include "platform.h"
#include "timer.h"
#include "adc_if.h"
#include "flash_if.h"

#include "systime.h"
#include "timer.h"
#include "stdlib.h"
#include "string.h"
#include "logger.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"

#include "lora_porting.h"
#include "logger.h"

#include "Commissioning.h"
#include "LmHandler.h"
#include "LmhpCompliance.h"
#include "LmhpClockSync.h"
#include "CayenneLpp.h"
#include "LmHandlerMsgDisplay.h"



#define JOINDELAY_EXPO 20000U
#define JOINDELAY_MAX  (10*60*1000U)


static const char *TAG = "WAN";
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE] = "HELLO";
static LmHandlerAppData_t AppData ={
    .Buffer = AppDataBuffer,
    .BufferSize = 5,
    .Port = LORAWAN_USER_APP_PORT
};

/**
 *  Control sleep mode
 */
static uint32_t nextjoin_delay = JOINDELAY_EXPO;




static void OnMacProcessNotify(void);
static void OnNvmDataChange(LmHandlerNvmContextStates_t state, uint16_t size);
static void OnNetworkParametersChange(CommissioningParams_t *params);
static void OnMacMcpsRequest(LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn);
static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn);
static void OnJoinRequest(LmHandlerJoinParams_t *params);
static void OnTxData(LmHandlerTxParams_t *params);
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);
static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection);
static void UplinkProcess(void);


static LmHandlerCallbacks_t LmHandlerCallbacks = {
    .GetBatteryLevel = GetBatteryLevel,
    .GetTemperature = NULL,
    .GetRandomSeed = GetRandomSeed,
    .OnMacProcess = OnMacProcessNotify,
    .OnNvmDataChange = OnNvmDataChange,
    .OnNetworkParametersChange = OnNetworkParametersChange,
    .OnMacMcpsRequest = OnMacMcpsRequest,
    .OnMacMlmeRequest = OnMacMlmeRequest,
    .OnJoinRequest = OnJoinRequest,
    .OnTxData = OnTxData,
    .OnRxData = OnRxData,
    .OnClassChange= NULL,
    .OnBeaconStatusChange = NULL,
    .OnSysTimeUpdate = OnSysTimeUpdate,
};

static LmHandlerParams_t LmHandlerParams = {
    .Region = ACTIVE_REGION,
    .AdrEnable = LORAWAN_ADR_STATE,
    .TxDatarate = LORAWAN_DEFAULT_DATA_RATE,
    .PublicNetworkEnable = LORAWAN_PUBLIC_NETWORK,
    .DutyCycleEnabled = LORAWAN_DUTYCYCLE_ON,
    .DataBufferMaxSize = LORAWAN_APP_DATA_BUFFER_MAX_SIZE,
    .DataBuffer = AppDataBuffer
};

static LmhpComplianceParams_t LmhpComplianceParams = {
    .AdrEnabled = LORAWAN_ADR_STATE,
    .DutyCycleEnabled = LORAWAN_DUTYCYCLE_ON,
    .StopPeripherals = NULL,
    .StartPeripherals = NULL,
};

TaskHandle_t htask_lmhandler;
static void Thd_LoraSendProcess(void *argument);
static void Thd_LmHandlerProcess(void *argument);



/**
 * **********************************************************************************************************************************************
 * LoRaWAN Initialize.
 */
void LoRaWAN_Init(void){
	if (xTaskCreate(Thd_LmHandlerProcess, "LmHandlerProcess",
			CFG_LM_HANDLER_PROCESS_STACK_SIZE/4, NULL, CFG_LM_HANDLER_PROCESS_PRIORITY, &htask_lmhandler) != pdTRUE)
		Error_Handler();

//	if (xTaskCreate(Thd_LoraSendProcess, "LoraSendProcess",
//			CFG_APP_LORA_PROCESS_STACK_SIZE/4, NULL, CFG_APP_LORA_PROCESS_PRIORITY, NULL) != pdTRUE)
//		Error_Handler();


    const Version_t appVersion = { .Fields.Major = 1, .Fields.Minor = 0, .Fields.Patch = 0 };
    DisplayAppInfo( "Callpoint", &appVersion );

    if (LmHandlerInit( &LmHandlerCallbacks, &LmHandlerParams ) != LORAMAC_HANDLER_SUCCESS){
        LOGE( TAG, "LoRaMac wasn't properly initialized\n" );
        Error_Handler();
    }
    LmHandlerSetSystemMaxRxError( 20 );
    LmHandlerPackageRegister( PACKAGE_ID_COMPLIANCE, &LmhpComplianceParams );
    LmHandlerPackageRegister( PACKAGE_ID_CLOCK_SYNC, &LmhpComplianceParams );
    LmHandlerJoin( );
}





/**
 * **********************************************************************************************************************************************
 * LoRaWAN Tasks.
 */
static void Thd_LmHandlerProcess(void *argument){
	UNUSED(argument);
	uint32_t notify_val;
	while(1){
		xTaskNotifyWait(0, 0, &notify_val, portMAX_DELAY);
		LmHandlerProcess( );
		(void)notify_val;
	}
}

static void Thd_LoraSendProcess(void *argument){
	UNUSED(argument);
	while(1){
		UplinkProcess( );
		vTaskDelay(APP_TX_DUTYCYCLE);
	}
}


static void UplinkProcess(void) {
	if (LmHandlerIsBusy() == true)
		return;

	if(LmHandlerJoinStatus() == LORAMAC_HANDLER_SET){
		if (LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE) == LORAMAC_HANDLER_SUCCESS) {

		}
	}
}



/**
 * **********************************************************************************************************************************************
 * LoRaWAN On event handlers.
 */
static void OnJoinRequest(LmHandlerJoinParams_t *params) {
	DisplayJoinRequestUpdate(params);
	if (params->Status == LORAMAC_HANDLER_ERROR) {
		if (nextjoin_delay >= JOINDELAY_MAX) {
			nextjoin_delay = UINT32_MAX;
			LOGE(TAG, "Join failed after 10mins, enter infinity loop");
		}
		EnterSleepMode(nextjoin_delay);
		nextjoin_delay += nextjoin_delay;
		LmHandlerJoin();
	}
	else {
		LmHandlerErrorStatus_t sync_status = LmhpClockSyncAppTimeReq();
		SysTime_t curTime = { .Seconds = 0, .SubSeconds = 0 };
		curTime = SysTimeGet( );
		LOGW(TAG, "Sync time status %d", sync_status);
		LOGW(TAG, "Curent time is %ds %dms", curTime.Seconds, curTime.SubSeconds);
	}
}

static void OnTxData(LmHandlerTxParams_t *params){
	DisplayTxUpdate( params );
}

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	DisplayRxUpdate(appData, params);
}

static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection) {
	LOGE(TAG, "OnSysTimeUpdate, Sync range=%d, Calibration value=%d", isSynchronized, timeCorrection);
	SysTime_t curTime = { .Seconds = 0, .SubSeconds = 0 };
	curTime = SysTimeGet( );
	LOGW(TAG, "Curent time is %ds %dms", curTime.Seconds, curTime.SubSeconds);
}

static void OnMacProcessNotify(void){
	if(__get_IPSR() == 0U) xTaskNotify(htask_lmhandler, 1, eNoAction);
	else {
		BaseType_t yield;
		xTaskNotifyFromISR(htask_lmhandler, 1, eNoAction, &yield);
		if(yield) portEND_SWITCHING_ISR (yield);
	}
}

static void OnNvmDataChange(LmHandlerNvmContextStates_t state, uint16_t size) {
	DisplayNvmDataChange(state, size);
}

static void OnNetworkParametersChange(CommissioningParams_t *params) {
	DisplayNetworkParametersUpdate(params);
}

static void OnMacMcpsRequest(LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn) {
	if (status == LORAMAC_STATUS_DUTYCYCLE_RESTRICTED) LOGE(TAG, "Next TX in %lu", nextTxIn);
//	DisplayMacMcpsRequestUpdate(status, mcpsReq, nextTxIn);
}

static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn) {
	DisplayMacMlmeRequestUpdate(status, mlmeReq, nextTxIn);
}



