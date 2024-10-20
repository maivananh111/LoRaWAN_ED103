

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

#define JOINDELAY_EXPO 			(20000U)
#define JOINDELAY_MAX  			(10*60*1000U)
#define REGULARCHECKS_PERIOD 	(60*1000U)
#define MAX_SEND_FAILED_COUNT 	(3U)
#define PRE_TO_ACTIVATION_TIME  (1*60*1000U)
#define PRE_ACTIVATION_DELAY    (30*1000U)

#define SESSION_BOOTUP		   	(1<<0U)
#define SESSION_REGULAR_CHECKS  (1<<1U)
#define SESSION_PREACTIVATION  	(1<<2U)
#define SESSION_ACTIVATION   	(1<<3U)


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

static uint32_t next_join_delay = JOINDELAY_EXPO;
static uint32_t curr_activation_time = 0;
static uint32_t next_activation_time = 0;
static uint8_t uplink_session_count = 0;
static uint8_t running_step = 0;
static bool is_firsttime_activation = true;

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
	xTaskCreate(Task_LmHandlerProcess, "Task_LmHandlerProcess", 4096/4, NULL, 20, &htask_lmhandler);
	xTaskCreate(Task_AppProcess, "Task_AppProcess", 8192/4, NULL, 10, &htask_app);

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
	static uint32_t session;
	vTaskSuspend(NULL);

	while(1){
		if (!LmHandlerIsBusy()){
			if (xSemaphoreTake(sem_newoperation_isready, portMAX_DELAY)) {
				LOGE(TAG, "******************************************************************************");
				if (xQueueReceive(que_timer_event, &session, portMAX_DELAY)) {
					TimerSetContext(&send_repeat_timer, &session);
					switch (session){
						case SESSION_BOOTUP:
							LOGV(TAG, "START SESSION BOOTUP");
							send_bootup();
						break;
						case SESSION_REGULAR_CHECKS:
							LOGV(TAG, "START SESSION REGULAR CHECKS");
							send_regularchecks();
						break;
						case SESSION_PREACTIVATION:
							LOGV(TAG, "START SESSION PREACTIVATION");
							send_preactivation();
						break;
						case SESSION_ACTIVATION:
							LOGV(TAG, "START SESSION ACTIVATION");
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
		if (next_join_delay >= JOINDELAY_MAX) {
			next_join_delay = UINT32_MAX;
			LOGE(TAG, "Join failed after 10mins, enter infinity loop");
		}
		EnterSleepModeOn(next_join_delay);
		next_join_delay += next_join_delay;
		LmHandlerJoin();
	}
	else {
		uint32_t bootup_event = SESSION_BOOTUP;
		xQueueSend(que_timer_event, &bootup_event, 100);
		vTaskResume(htask_app);
	}
}

static void OnTxData(LmHandlerTxParams_t *params){
	DisplayTxUpdate(params);

	if (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET){
		if (uplink_session_count < MAX_SEND_FAILED_COUNT){
			uint32_t nexttx = (getRandom()%7 + 3)*1000;
			TimerSetValue(&send_repeat_timer, (getRandom()%7 + 3)*1000);
			TimerStart(&send_repeat_timer);
			LOGW(TAG, "REPEAT UPLINK SESSION AFTER %dms", nexttx);
		}
		else {
			LOGW(TAG, "THIS IS THE LAST ATTEMP TO UPLINK OF SESSION");
		}
	}
}

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	bool sleep_after_rx = false;
	uint32_t new_next_activation_time = 0;
	DisplayRxUpdate(appData, params);

	if (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET){
		if (appData->BufferSize != 0){
			uint8_t command = appData->Buffer[0];

			switch(command){
				case APPPACK_RES_BOOT_UP: {
					LOGW(TAG, "RECEIVE APPPACK_RES_BOOT_UP");
					TimerSetValue(&regularcheck_timer, REGULARCHECKS_PERIOD);
					TimerStart(&regularcheck_timer);
				}
				break;

				case APPPACK_RES_REGULAR_CHECK: {
					LOGW(TAG, "RECEIVE APPPACK_RES_REGULAR_CHECK");

					apppack_res_regularcheck_t res;
					if (apppack_res_regularchecks_parse(appData->Buffer, appData->BufferSize, &res))
						new_next_activation_time = res.next_trig_time;
					else goto end_session;
				}
				break;
				case APPPACK_RES_PREACTIVATION_CHECK: {
					LOGW(TAG, "RECEIVE APPPACK_RES_PREACTIVATION_CHECK");

					apppack_res_preactivation_t res;
					if (apppack_res_preactivation_parse(appData->Buffer, appData->BufferSize, &res))
						new_next_activation_time = res.next_trig_time;
					else goto end_session;
				}
				break;
				case APPPACK_REQ_ACTIVATION_STATUSACK:
					LOGW(TAG, "RECEIVE APPPACK_REQ_ACTIVATION_STATUSACK");

					apppack_res_activation_t res;
					if (apppack_res_activation_parse(appData->Buffer, appData->BufferSize, &res))
						new_next_activation_time = res.next_trig_time;
					else goto end_session;
				break;
				default:
					return;
				break;
			}
		}
		else{
			if(uplink_session_count >= MAX_SEND_FAILED_COUNT){
				LOGE(TAG, "SESSION FAILED");
				goto end_session;
			}
			return;
		}


		SysTime_t curTime = SysTimeGet();
		if (new_next_activation_time > curTime.Seconds
				&& new_next_activation_time > (curr_activation_time + PRE_ACTIVATION_DELAY + PRE_TO_ACTIVATION_TIME)){
			next_activation_time = new_next_activation_time;

			if (is_firsttime_activation == true){
				is_firsttime_activation = false;
				curr_activation_time = next_activation_time;
				LOGE(TAG, "PREACTIVATION AT %lus, ACTIVATION AT %lus",
						curr_activation_time - PRE_TO_ACTIVATION_TIME - curTime.Seconds, curr_activation_time - curTime.Seconds);
				TimerSetValue(&preactivation_timer, curr_activation_time - PRE_TO_ACTIVATION_TIME - curTime.Seconds);
				TimerStart(&preactivation_timer);
				TimerSetValue(&activation_timer, curr_activation_time - curTime.Seconds);
				TimerStart(&activation_timer);
			}
		}

end_session:
		TimerStop(&send_repeat_timer);
		if(xSemaphoreGive(sem_newoperation_isready) != pdPASS) LOGE(TAG, "SEMAPHORE ERROR");
		uplink_session_count = 0;
		vTaskResume(htask_app);
		LOGV(TAG, "END SESSION");
		LOGE(TAG, "******************************************************************************");

		if (sleep_after_rx) EnterSleepMode();
	}
}


static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection) {
	SysTime_t curTime = SysTimeGet();
	LOGV(TAG, "System time has been synchronized, current time is %ds %dms", curTime.Seconds, curTime.SubSeconds);
}



/**
 * **********************************************************************************************************************************************
 * Application timers handlers.
 */

static void TimerOnRepeatSend(void *session) {
	uint32_t *type = (uint32_t *)session;
	LmHandlerErrorStatus_t status;

	uplink_session_count++;
	TimerStop(&send_repeat_timer);

	if (uplink_session_count <= MAX_SEND_FAILED_COUNT) {
		status = (*type == SESSION_BOOTUP)? LmhpClockSyncAppTimeReq() : LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE);

		if (status != LORAMAC_HANDLER_SUCCESS)
			LOGE(TAG, "SENT UPLINK FAILED");
		else {
			LOGV(TAG, "SENT UPLINK");
			if (*type == SESSION_BOOTUP) {
				SysTime_t curTime = SysTimeGet();
				LOGW(TAG, "Sync time status %d, curent time is %ds %dms", status, curTime.Seconds, curTime.SubSeconds);
			}
		}
	}
}

static void TimerOnRegularCheck(void *){
	BaseType_t yield;
	uint32_t session = SESSION_REGULAR_CHECKS;
	xQueueSendFromISR(que_timer_event, &session, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
	TimerReset(&regularcheck_timer);
}

static void TimerOnPreActivation(void *){
	BaseType_t yield;
	uint32_t session = SESSION_PREACTIVATION;
	xQueueSendFromISR(que_timer_event, &session, &yield);
	if(yield) portYIELD_FROM_ISR(yield);

	TimerStop(&preactivation_timer);
}

static void TimerOnActivation(void *){
	BaseType_t yield;
	uint32_t session = SESSION_ACTIVATION;
	xQueueSendFromISR(que_timer_event, &session, &yield);
	if(yield) portYIELD_FROM_ISR(yield);

	TimerStop(&activation_timer);
	SysTime_t curTime = SysTimeGet();
	curr_activation_time = next_activation_time;
	LOGE(TAG, "PREACTIVATION AT %lus, ACTIVATION AT %lus",
							curr_activation_time - PRE_TO_ACTIVATION_TIME - curTime.Seconds, curr_activation_time - curTime.Seconds);
	TimerSetValue(&preactivation_timer, curr_activation_time - PRE_TO_ACTIVATION_TIME - curTime.Seconds);
	TimerStart(&preactivation_timer);
	TimerSetValue(&activation_timer, curr_activation_time - curTime.Seconds);
	TimerStart(&activation_timer);
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
}

static void send_regularchecks(void){
	apppack_req_regularcheck_t pack_regcheck = {
		.devtype = DEVICE,
	};
	AppData.BufferSize = apppack_req_regularchecks_create(&pack_regcheck, AppDataBuffer);
}

static void send_preactivation(void){
	apppack_req_preactivation_t pack_preactivation = {
		.devtype = DEVICE,
	};
	AppData.BufferSize = apppack_req_preactivation_create(&pack_preactivation, AppDataBuffer);
}

static void send_activation(void){
	apppack_req_activation_t pack_activation = {
		.devtype = DEVICE,
		.runstep = running_step,
	};
	AppData.BufferSize = apppack_req_activation_create(&pack_activation, AppDataBuffer);
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













