

#include "lora_apps.h"
#include "app_version.h"
#include "lora_porting.h"
#include "app_packketstructs.h"
#include "stdlib.h"
#include "string.h"
#include "platform.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"

#include "adc_if.h"
#include "flash_if.h"
#include "systime.h"
#include "timer.h"
#include "logger.h"

#include "Commissioning.h"
#include "LmHandler.h"
#include "LmhpCompliance.h"
#include "LmhpClockSync.h"
#include "CayenneLpp.h"
#include "LmHandlerMsgDisplay.h"


#define DEVICE CALLPOINT_ACTUATOR

#define JOINDELAY_EXPO 			20000U
#define JOINDELAY_MAX  			(10*60*1000U)
#define REGULARCHECKS_PERIOD 	(60*1000U)
#define MAX_SEND_FAILED_COUNT 	3U

#define EVENT_BOOTUP		   	(1<<0U)
#define EVENT_REGULAR_CHECKS  	(1<<1U)
#define EVENT_PREACTIVATION  	(1<<2U)
#define EVENT_ACTIVATION   		(1<<3U)


static const char *TAG = "WAN";
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];
static LmHandlerAppData_t AppData ={
    .Buffer = AppDataBuffer,
    .BufferSize = 5,
    .Port = LORAWAN_USER_APP_PORT
};

static void OnMacProcessNotify(void);
static void OnNvmDataChange(LmHandlerNvmContextStates_t state, uint16_t size);
static void OnNetworkParametersChange(CommissioningParams_t *params);
static void OnMacMcpsRequest(LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn);
static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn);
static void OnJoinRequest(LmHandlerJoinParams_t *params);
static void OnTxData(LmHandlerTxParams_t *params);
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);
static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection);


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

static TaskHandle_t htask_lmhandler;
static TaskHandle_t htask_app;
static QueueHandle_t que_timer_event;
static SemaphoreHandle_t sem_newoperation_isready;

static uint32_t nextjoin_delay = JOINDELAY_EXPO;
static uint8_t send_failed_count = 0;
static uint8_t running_step = 0;


static TimerEvent_t send_repeat_timer;
static TimerEvent_t regularcheck_timer;
static TimerEvent_t preactivation_timer;
static TimerEvent_t activation_timer;


static void Task_AppProcess(void *);
static void Task_LmHandlerProcess(void *);

static void TimerOnRepeatSend(void *);
static void TimerOnRegularCheck(void *);
static void TimerOnPreActivation(void *);
static void TimerOnActivation(void *);

static void send_bootup(void);
static void send_regularchecks(void);
static void send_preactivation(void);
static void send_activation(void);

/**
 * **********************************************************************************************************************************************
 * LoRaWAN Initialize.
 */
void LoRaWAN_Init(void){
	que_timer_event = xQueueCreate(5, sizeof(uint32_t));
	sem_newoperation_isready = xSemaphoreCreateBinary();
	xSemaphoreGive(sem_newoperation_isready);
	if (xTaskCreate(Task_LmHandlerProcess, "Task_LmHandlerProcess", 4096/4, NULL, 20, &htask_lmhandler) != pdTRUE)
		Error_Handler();

	if (xTaskCreate(Task_AppProcess, "Task_AppProcess", 8192/4, NULL, 10, &htask_app) != pdTRUE)
		Error_Handler();



	TimerInit(&send_repeat_timer, 	TimerOnRepeatSend);
	TimerInit(&regularcheck_timer, 	TimerOnRegularCheck);
	TimerInit(&preactivation_timer, TimerOnPreActivation);
	TimerInit(&activation_timer, 	TimerOnActivation);



    const Version_t appVersion = {
		.Fields.Major = APP_VERSION_MAJOR,
		.Fields.Minor = APP_VERSION_MINOR,
		.Fields.Patch = APP_VERSION_REVISION
    };
	DisplayAppInfo("Callpoint", &appVersion);

    if (LmHandlerInit( &LmHandlerCallbacks, &LmHandlerParams ) != LORAMAC_HANDLER_SUCCESS){
        LOGE( TAG, "LoRaMac wasn't properly initialized\n" );
        Error_Handler();
    }
    LmHandlerSetSystemMaxRxError( 20 );
    LmHandlerPackageRegister( PACKAGE_ID_COMPLIANCE, &LmhpComplianceParams );
    LmHandlerPackageRegister( PACKAGE_ID_CLOCK_SYNC, &LmhpComplianceParams );
    LmHandlerJoin();
}





/**
 * **********************************************************************************************************************************************
 * LoRaWAN Tasks.
 */

static void Task_AppProcess(void *){
	static uint32_t event;
	vTaskSuspend(NULL);

	while(1){
		if (xSemaphoreTake(sem_newoperation_isready, portMAX_DELAY)) {
			LOGE(TAG, "*****************************************************************");
			if (xQueueReceive(que_timer_event, &event, portMAX_DELAY)) {
				LOGE(TAG, "QUEUE EVENT %d", event);
				TimerSetContext(&send_repeat_timer, &event);
				switch (event){
					case EVENT_BOOTUP:
						send_bootup();
					break;
					case EVENT_REGULAR_CHECKS:
						send_regularchecks();
					break;
					case EVENT_PREACTIVATION:
						send_preactivation();
					break;
					case EVENT_ACTIVATION:
						send_activation();
					break;
					default:
					break;
				}
				TimerSetValue(&send_repeat_timer, 200);
				TimerStart(&send_repeat_timer);
				vTaskSuspend(NULL);
				EnterSleepMode();
			}
		}
	}
}

static void Task_LmHandlerProcess(void *){
	uint32_t notify_val;
	while(1){
		xTaskNotifyWait(0, 0, &notify_val, portMAX_DELAY);
		LmHandlerProcess( );
		(void)notify_val;
	}
}






/**
 * **********************************************************************************************************************************************
 * LoRaWAN On event handlers part 2.
 */
static void OnJoinRequest(LmHandlerJoinParams_t *params) {
	DisplayJoinRequestUpdate(params);
	if (params->Status == LORAMAC_HANDLER_ERROR) {
		if (nextjoin_delay >= JOINDELAY_MAX) {
			nextjoin_delay = UINT32_MAX;
			LOGE(TAG, "Join failed after 10mins, enter infinity loop");
		}
		EnterSleepModeOn(nextjoin_delay);
		nextjoin_delay += nextjoin_delay;
		LmHandlerJoin();
	}
	else {
		LmHandlerErrorStatus_t sync_status = LmhpClockSyncAppTimeReq();
		LOGW(TAG, "Sync time status %d", sync_status);
		SysTime_t curTime = { .Seconds = 0, .SubSeconds = 0 };
		curTime = SysTimeGet( );
		LOGW(TAG, "Curent time is %ds %dms", curTime.Seconds, curTime.SubSeconds);
	}
}

static void OnTxData(LmHandlerTxParams_t *params){
	DisplayTxUpdate( params );
}

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	DisplayRxUpdate(appData, params);

	if (appData->BufferSize != 0){
		uint8_t command = appData->Buffer[0];

		switch(command){
			case APPPACK_RES_BOOT_UP:
				LOGV(TAG, "RECEIVE APPPACK_RES_BOOT_UP");
				TimerSetValue(&regularcheck_timer, REGULARCHECKS_PERIOD);
				TimerStart(&regularcheck_timer);
				EnterSleepMode();
			break;
			case APPPACK_RES_REGULAR_CHECK:
				LOGV(TAG, "RECEIVE APPPACK_RES_REGULAR_CHECK");
			break;
			case APPPACK_RES_PREACTIVATION_CHECK:

			break;
			case APPPACK_REQ_ACTIVATION_STATUSACK:

			break;
			default:

			break;
		}

		if (command == APPPACK_RES_BOOT_UP ||
			command == APPPACK_RES_REGULAR_CHECK ||
			command == APPPACK_RES_PREACTIVATION_CHECK ||
			command == APPPACK_REQ_ACTIVATION_STATUSACK) {
			TimerStop(&send_repeat_timer);

			if(!xPortIsInsideInterrupt()){
				if(xSemaphoreGive(sem_newoperation_isready) != pdPASS)
					LOGE(TAG, "SEMAPHORE ERROR");
			}
			else {
				BaseType_t yield;
				if(xSemaphoreGiveFromISR(sem_newoperation_isready, &yield) != pdPASS)
					LOGE(TAG, "SEMAPHORE IT ERROR");
			}
			send_failed_count = 0;
			vTaskResume(htask_app);
		}
	}
}


static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection) {
	LOGW(TAG, "OnSysTimeUpdate, Sync range=%d, Calibration value=%d", isSynchronized, timeCorrection);
	SysTime_t curTime = { .Seconds = 0, .SubSeconds = 0 };
	curTime = SysTimeGet( );
	LOGW(TAG, "Curent time is %ds %dms", curTime.Seconds, curTime.SubSeconds);

	uint32_t bootup_event = EVENT_BOOTUP;
	xQueueSend(que_timer_event, &bootup_event, 100);
	vTaskResume(htask_app);
}



/**
 * **********************************************************************************************************************************************
 * Application timers handlers.
 */

static void TimerOnRepeatSend(void *eventtype) {
	send_failed_count++;
	TimerStop(&send_repeat_timer);

	uint32_t *event = (uint32_t *)eventtype;
	LOGV(TAG, "SEND PACKET FOR EVENT 0x%02x", (uint32_t)(*event));

	if(send_failed_count < MAX_SEND_FAILED_COUNT){
		if (LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE) != LORAMAC_HANDLER_SUCCESS) {
			LOGE(TAG, "Send uplink failed");
		}
	}

	if(send_failed_count < MAX_SEND_FAILED_COUNT){
		uint32_t nexttx = (getRandom()%7 + 3)*1000;
		TimerSetValue(&send_repeat_timer, (getRandom()%7 + 3)*1000);
		TimerStart(&send_repeat_timer);
		LOGV(TAG, "REPEAT TX AFTER %dms", nexttx);
//		EnterSleepMode();
	}
	else{
		BaseType_t yield;
		xSemaphoreGiveFromISR(sem_newoperation_isready, &yield);
		if(yield) portEND_SWITCHING_ISR (yield);
		send_failed_count = 0;
		xTaskResumeFromISR(htask_app);
		LOGE(TAG, "EVENT %d SEND FAILED, IGNORING", (uint32_t)(*event));
	}
}

static void TimerOnRegularCheck(void *){
	BaseType_t yield;
	uint32_t event = EVENT_REGULAR_CHECKS;
	xQueueSendFromISR(que_timer_event, &event, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
	TimerReset(&regularcheck_timer);
}

static void TimerOnPreActivation(void *){
	BaseType_t yield;
	uint32_t event = EVENT_PREACTIVATION;
	xQueueSendFromISR(que_timer_event, &event, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
}

static void TimerOnActivation(void *){
	BaseType_t yield;
	uint32_t event = EVENT_ACTIVATION;
	xQueueSendFromISR(que_timer_event, &event, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
}





/**
 * **********************************************************************************************************************************************
 * Application packet handlers.
 */
static void send_bootup(void){
	apppack_req_bootup_t pack_bootup = {
		.devtype 		= DEVICE,
		.appver_major 	= APP_VERSION_MAJOR,
		.appver_minor 	= APP_VERSION_MINOR,
		.appver_rev   	= APP_VERSION_REVISION,
	};

	AppData.BufferSize = apppack_req_bootup_create(&pack_bootup, AppDataBuffer);
	LOGV(TAG, "SEND BOOTUP");
}

static void send_regularchecks(void){
	apppack_req_regularcheck_t pack_regcheck = {
		.devtype = DEVICE,
	};
	AppData.BufferSize = apppack_req_regularchecks_create(&pack_regcheck, AppDataBuffer);
	LOGV(TAG, "SEND REGULAR CHECK");
}

static void send_preactivation(void){
	apppack_req_preactivation_t pack_preactivation = {
		.devtype = DEVICE,
	};
	AppData.BufferSize = apppack_req_preactivation_create(&pack_preactivation, AppDataBuffer);
	LOGV(TAG, "SEND PREACTIVATION CHECK");
}

static void send_activation(void){
	apppack_req_activation_t pack_activation = {
		.devtype = DEVICE,
		.runstep = running_step,
	};
	AppData.BufferSize = apppack_req_activation_create(&pack_activation, AppDataBuffer);
	LOGV(TAG, "SEND ACTIVATION");
}








/**
 * **********************************************************************************************************************************************
 * LoRaWAN On event handlers part 2.
 */

static void OnMacProcessNotify(void){
	if(!xPortIsInsideInterrupt()) xTaskNotify(htask_lmhandler, 1, eNoAction);
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













