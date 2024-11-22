

#include "lora_apps.h"
#include "app_version.h"
#include "lora_porting.h"
#include "app_msgstructs.h"
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


#define DEVICE 										CALLPOINT_ACTUATOR
#define APPLICATION 								"Callpoint Actuator"

#define JOINDELAY_EXPO 								(20000U)
#define JOINDELAY_MAX  								(10*60*1000U)
#define REGULARCHECKS_PERIOD 						(2*60*1000U)
#define DEFAUT_MAX_SEND_COUNT 						(3U)
#define PRE_TO_ACTIVATION_TIME 						(15*60)
#define PRE_ACTIVATION_DELAY 						(60)
#define RETRY_TOSEND_UPLINK_DELAY_MIN 				3
#define RETRY_TOSEND_UPLINK_DELAY_MAX 				10
#define LAST_RUN_STEP 								7U

#define SESSION_BOOTUP		   						(uint8_t)APPMSG_REQ_BOOT_UP
#define SESSION_REGULAR_CHECKS  					(uint8_t)APPMSG_REQ_REGULAR_CHECK
#define SESSION_PREACTIVATION  						(uint8_t)APPMSG_REQ_PREACTIVATION_CHECK
#define SESSION_ACTIVATION   						(uint8_t)APPMSG_REQ_ACTIVATION_STATUS
#define SESSION_STATUSPOLL   						(uint8_t)APPMSG_REQ_ACTIVATION_STATUS




static const char *TAG = "LoRaWAN";
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];
static LmHandlerAppData_t AppData = {
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
static void OnLinkCheck(MlmeConfirm_t *mlmeConfirm);

static void OnTxPeriodicityChanged( uint32_t periodicity );
static void OnTxFrameCtrlChanged( LmHandlerMsgTypes_t isTxConfirmed );
static void OnPingSlotPeriodicityChanged( uint8_t pingSlotPeriodicity );

const Version_t appVersion = {
	.Fields.Major 				= APP_VERSION_MAJOR,
	.Fields.Minor 				= APP_VERSION_MINOR,
	.Fields.Patch 				= APP_VERSION_REVISION
};

static LmHandlerCallbacks_t LmHandlerCallbacks = {
    .GetBatteryLevel 			= GetBatteryLevel,
    .GetTemperature 			= NULL,
    .GetRandomSeed 				= GetRandomSeed,
    .OnMacProcess 				= OnMacProcessNotify,
    .OnNvmDataChange 			= OnNvmDataChange,
    .OnNetworkParametersChange 	= OnNetworkParametersChange,
    .OnMacMcpsRequest 			= OnMacMcpsRequest,
    .OnMacMlmeRequest 			= OnMacMlmeRequest,
    .OnJoinRequest 				= OnJoinRequest,
    .OnTxData 					= OnTxData,
    .OnRxData 					= OnRxData,
    .OnClassChange				= NULL,
    .OnBeaconStatusChange 		= NULL,
    .OnSysTimeUpdate 			= OnSysTimeUpdate,
	.OnLinkCheck				= OnLinkCheck,
};

static LmHandlerParams_t LmHandlerParams = {
    .Region 					= ACTIVE_REGION,
    .AdrEnable 					= LORAWAN_ADR_STATE,
    .TxDatarate 				= LORAWAN_DEFAULT_DATA_RATE,
    .PublicNetworkEnable 		= LORAWAN_PUBLIC_NETWORK,
    .DutyCycleEnabled 			= LORAWAN_DUTYCYCLE_ON,
    .DataBufferMaxSize 			= LORAWAN_APP_DATA_BUFFER_MAX_SIZE,
    .DataBuffer 				= AppDataBuffer
};

static LmhpComplianceParams_t LmhpComplianceParams ={
    .FwVersion.Value = 0x01030000 ,
    .OnTxPeriodicityChanged = OnTxPeriodicityChanged,
    .OnTxFrameCtrlChanged = OnTxFrameCtrlChanged,
    .OnPingSlotPeriodicityChanged = OnPingSlotPeriodicityChanged,
};

static TaskHandle_t htask_lmhandler;
static TaskHandle_t htask_app;
static TaskHandle_t htask_activation;

static QueueHandle_t queue_timer_session;
static SemaphoreHandle_t sem_newoperation_isready;
static SemaphoreHandle_t sem_nextsend_isready;

static uint32_t next_join_delay = JOINDELAY_EXPO;
static uint32_t curr_activation_time = 0;
static uint32_t uplink_session_count = 0;
static uint32_t max_send_count = DEFAUT_MAX_SEND_COUNT;
static uint8_t running_step = 0;
static bool enable_regular_check = false;
static bool enable_activation = false;
static bool linkcheck_request = false;
static bool linkcheck_confirm = false;
static bool time_synchronized = false;

static TimerEvent_t send_repeat_timer;
static TimerEvent_t regularcheck_timer;
static TimerEvent_t preactivation_timer;
static TimerEvent_t activation_timer;

static const char *SessionString[] = {
	[APPMSG_REQ_BOOT_UP] 				= "Start session bootup",
	[APPMSG_REQ_REGULAR_CHECK] 			= "Start session regular checks",
	[APPMSG_REQ_PREACTIVATION_CHECK] 	= "Start session preactivation",
	[APPMSG_REQ_ACTIVATION_STATUS] 		= "Start session activation",
};

static const char *SessionResponsedString[] = {
	[APPMSG_RES_REGULAR_CHECK] 			= "Receive regular checks message response",
	[APPMSG_RES_PREACTIVATION_CHECK] 	= "Receive preactivation message response",
	[APPMSG_RES_ACTIVATION_STATUSACK] 	= "Receive activation-ACK message response",
	[APPMSG_RES_BOOT_UP] 				= "Receive bootup message response",
};


static void Task_AppProcess(void *);
static void Task_Activation(void *);
static void Task_LmHandlerProcess(void *);

static void send_uplink_message(appmsg_types_t type);
static void run_next_activation_step(void);

static void TimerOnRepeatSend(void *);
static void TimerOnRegularCheck(void *);
static void TimerOnPreActivation(void *);
static void TimerOnActivation(void *);


static void rejoin_exponential_backoff(void);
static void joined_startapp(void);
static void startsession_blockapp(appmsg_types_t session);
static void stopsession_releaseapp(void);
static void prepare_tosend_uplink(void);
static void attemp_tosend_uplink(appmsg_types_t session);
static void rejoin_to_network(void);
static void newactivationtime_apply_and_starttimer(uint32_t new_next_act);

/**
 * **********************************************************************************************************************************************
 * LoRaWAN Initialize.
 */
void LoRaWAN_Init(void){
	enable_regular_check = false;
	enable_activation = false;
	max_send_count = DEFAUT_MAX_SEND_COUNT;
	running_step = 0;

	queue_timer_session = xQueueCreate(5, sizeof(appmsg_types_t *));
	sem_newoperation_isready = xSemaphoreCreateBinary();
	sem_nextsend_isready = xSemaphoreCreateBinary();
	xSemaphoreGive(sem_newoperation_isready);
	xSemaphoreGive(sem_nextsend_isready);
	xTaskCreate(Task_LmHandlerProcess, 	"Task_LmHandlerProcess", 	4096/4, NULL, 20, &htask_lmhandler);
	xTaskCreate(Task_AppProcess, 		"Task_AppProcess", 			8192/4, NULL, 10, &htask_app);
	xTaskCreate(Task_Activation, 		"Task_Activation", 			4096/4, NULL, 25, &htask_activation);


	TimerInit(&send_repeat_timer, 	TimerOnRepeatSend);
	TimerInit(&regularcheck_timer, 	TimerOnRegularCheck);
	TimerInit(&preactivation_timer, TimerOnPreActivation);
	TimerInit(&activation_timer, 	TimerOnActivation);
	EnableSleepMode(true);

	DisplayAppInfo(APPLICATION, &appVersion);

    if (LmHandlerInit( &LmHandlerCallbacks, &LmHandlerParams ) != LORAMAC_HANDLER_SUCCESS){
        LOGE( TAG, "LoRaMac wasn't properly initialized\n" );
        Error_Handler();
    }

    LmHandlerSetSystemMaxRxError( 100 );
    LmHandlerPackageRegister( PACKAGE_ID_COMPLIANCE, &LmhpComplianceParams );
    LmHandlerPackageRegister( PACKAGE_ID_CLOCK_SYNC, &LmhpComplianceParams );
    LmHandlerJoin();
}





/**
 * **********************************************************************************************************************************************
 * LoRaWAN Tasks.
 */

static void Task_AppProcess(void *){
	appmsg_types_t session_bootup = SESSION_BOOTUP;
	static appmsg_types_t session;
	vTaskSuspend(NULL);

	max_send_count = UINT32_MAX;
	xQueueSend(queue_timer_session, (void *)&session_bootup, 100);

	while (1){
		if (!LmHandlerIsBusy()){
			if (xSemaphoreTake(sem_newoperation_isready, portMAX_DELAY)){
				if (xQueueReceive(queue_timer_session, (void *)&session, portMAX_DELAY))
					startsession_blockapp(session);
			}
		}
	}
}


static void Task_Activation(void *){
	vTaskSuspend(NULL);

	while (1){
		run_next_activation_step();

		if (running_step == LAST_RUN_STEP){
			max_send_count = UINT8_MAX;
			EnableSleepMode(true);
			EnterSleepMode();
			vTaskSuspend(NULL);
		}
	}
}






/**
 * **********************************************************************************************************************************************
 * LoRaWAN On event handlers part 2.
 */
static void OnJoinRequest(LmHandlerJoinParams_t *params) {
	DisplayJoinRequestUpdate(params);

	if (params->Status == LORAMAC_HANDLER_ERROR)
		rejoin_exponential_backoff();
	else {
		joined_startapp();
	}
}


static void OnTxData(LmHandlerTxParams_t *params){
	if (!params->IsMcpsConfirm){
		DisplayTxUpdate(params);

		prepare_tosend_uplink();
	}
}


static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	uint32_t next_activation_time = 0;
	DisplayRxUpdate(appData, params);

	if (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET){
		if (appData->BufferSize != 0){
			appmsg_response_t response;
			if (appmsg_parse_message(appData->Buffer, appData->BufferSize, &response)){
				LOGV(TAG, "%s", SessionResponsedString[response.resmsg_type]);
				TimerSetValue(&regularcheck_timer, REGULARCHECKS_PERIOD);
				TimerStart(&regularcheck_timer);
				max_send_count = DEFAUT_MAX_SEND_COUNT;
				enable_regular_check = true;
				next_activation_time = response.next_trig_time;

				if (response.resmsg_type == APPMSG_RES_PREACTIVATION_CHECK) {
					if (response.action_confimed)
						LOGI(TAG, "Activation confirmed, continue.");
					else
						LOGI(TAG, "Activation unconfirmed, stop.");
				}
			}
			else
				goto session_done;
		}
		else {
			if(uplink_session_count >= max_send_count){
				LOGE(TAG, "Session failed, rejoin");
				rejoin_to_network();
			}
			return;
		}
		newactivationtime_apply_and_starttimer(next_activation_time);

session_done:
		stopsession_releaseapp();
	}
}




/**
 * **********************************************************************************************************************************************
 * Application timers handlers.
 */

static void TimerOnRepeatSend(void *session) {
	appmsg_types_t *sess = (appmsg_types_t *)session;
	attemp_tosend_uplink(*sess);
}

static void TimerOnRegularCheck(void *){
	BaseType_t yield;
	appmsg_types_t session = SESSION_REGULAR_CHECKS;
	xQueueSendFromISR(queue_timer_session, (void *)&session, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
	TimerReset(&regularcheck_timer);
}

static void TimerOnPreActivation(void *){
	BaseType_t yield;
	appmsg_types_t session = SESSION_PREACTIVATION;
	xQueueSendFromISR(queue_timer_session, (void *)&session, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
	TimerStop(&preactivation_timer);
}

static void TimerOnActivation(void *){
	TimerStop(&activation_timer);

	EnableSleepMode(false);
	xTaskResumeFromISR(htask_activation);
	running_step = 0;
}

static void OnSysTimeUpdate( bool isSynchronized, int32_t timeCorrection) {
	SysTime_t curTime = SysTimeGet();
	LOGV(TAG, "System time has been synchronized, current time is %ds %dms", curTime.Seconds, curTime.SubSeconds);
	time_synchronized = true;
}

static void OnLinkCheck(MlmeConfirm_t *mlmeConfirm) {
	LOGI(TAG, "Link check confirm, margin:%hu, nbgateways:%hu",
			mlmeConfirm->DemodMargin, mlmeConfirm->NbGateways);
	linkcheck_confirm = true;
}


/**
 * **********************************************************************************************************************************************
 * Application functions.
 */
static void send_uplink_message(appmsg_types_t type){
	float batt_adc = GetBatteryVol();

	appmsg_request_t msg = {
		.dev_type 		= DEVICE,
		.appver_major 	= APP_VERSION_MAJOR,
		.appver_minor 	= APP_VERSION_MINOR,
		.appver_rev   	= APP_VERSION_REVISION,
		.reqmsg_type    = type,
		.run_step 		= running_step,
		.regularcheck_freq = 2,
		.battery_vol = (uint8_t)(batt_adc / 0.1),
	};

	AppData.BufferSize = appmsg_create_message(&msg, AppDataBuffer);
}


static void run_next_activation_step(void){
	appmsg_types_t session = SESSION_ACTIVATION;

	running_step++;
	if (running_step == LAST_RUN_STEP) return;
	LOGI(TAG, "Run motor step %hu", running_step);

	if (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET)
		xQueueSend(queue_timer_session, (void *)&session, 50);

	switch (running_step) {
		case 1: /** cc: Bắt đầu đẩy ra */
			HAL_GPIO_WritePin(EN_12V_GPIO_Port, EN_12V_Pin, GPIO_PIN_SET);
			vTaskDelay(1000);
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 0);
			HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, 1);
			vTaskDelay(5000); /** Thời gian chạy các bước, 10s là ước tính dựa trên bản demo 10 năm trước */
		break;
		case 2: /** cc: Bắt đầu giữ lại ở điểm cao nhất */
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 0);
			HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, 0);
			vTaskDelay(5000); /** Thời gian chạy các bước, 5s là ước tính dựa trên bản demo 10 năm trước */
		break;
		case 3: /** cc: Bắt đầu kéo về */
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 1);
			HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, 0);
			vTaskDelay(5000); /** Thời gian chạy các bước, 10s là ước tính dựa trên bản demo 10 năm trước */
		break;
		case 4: /** cc: Kéo về xong, kết thúc */
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 0);
			HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, 0);
			HAL_GPIO_WritePin(EN_12V_GPIO_Port, EN_12V_Pin, GPIO_PIN_RESET);
		break;
		default:
		break;
	};
}





static void rejoin_exponential_backoff(void) {
	if (next_join_delay >= JOINDELAY_MAX) {
		next_join_delay = UINT32_MAX;
		LOGE(TAG, "Join failed after 10mins, enter sleep mode forever");
	}

	EnterSleepModeOn(next_join_delay);

	if(next_join_delay <= JOINDELAY_MAX)
		next_join_delay += next_join_delay;
	LmHandlerJoin();
}

static void joined_startapp(void) {
	if (enable_activation) {
		TimerStart(&preactivation_timer);
		TimerStart(&activation_timer);
	}

	if (enable_regular_check) {
		TimerStart(&regularcheck_timer);
		if(xSemaphoreGive(sem_newoperation_isready) != pdPASS)
			LOGE(TAG, "Semaphore give failed"); /** cc: Lâu lâu bug ở đây */
	}

	vTaskResume(htask_app);
}



static void prepare_tosend_uplink(void) {
	if (LmHandlerJoinStatus() == LORAMAC_HANDLER_SET && xSemaphoreTake(sem_nextsend_isready, 50)){
		if (uplink_session_count < max_send_count){
			uint32_t nexttx = (getRandom()%(RETRY_TOSEND_UPLINK_DELAY_MAX - RETRY_TOSEND_UPLINK_DELAY_MIN) + RETRY_TOSEND_UPLINK_DELAY_MIN) * 1000U;
			LOGW(TAG, "Next uplink message will be send after %dms", nexttx);
			TimerSetValue(&send_repeat_timer, nexttx);
			TimerStart(&send_repeat_timer);
		}
		else
			LOGW(TAG, "Tried again 3 times but got nothing");
	}
}

static void attemp_tosend_uplink(appmsg_types_t session) {
	LmHandlerErrorStatus_t status;

	if (linkcheck_request == true && linkcheck_confirm == false) {
		LOGE(TAG, "The connection between the device and the gateway is lost, rejoin");
		rejoin_to_network();
	}

	uplink_session_count++;
	TimerStop(&send_repeat_timer);

	if (uplink_session_count <= max_send_count) {
		MlmeReq_t mlmeReq;
		if (session == SESSION_BOOTUP)
			mlmeReq.Type = MLME_DEVICE_TIME;
		else if (session == SESSION_REGULAR_CHECKS) {
			mlmeReq.Type = MLME_LINK_CHECK;
			linkcheck_request = true;
		}

		status = LoRaMacMlmeRequest( &mlmeReq );
		if (status != LORAMAC_HANDLER_SUCCESS)
			LOGE(TAG, "Add mlme request failed");
		status = LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE);
		if (status != LORAMAC_HANDLER_SUCCESS)
			LOGE(TAG, "Send uplink message failed");

		else {
			LOGV(TAG, "Sent uplink");
			if (session == SESSION_BOOTUP) {
				SysTime_t curTime = SysTimeGet();
				LOGI(TAG, "Sync time status %d, curent time is %ds %dms", status, curTime.Seconds, curTime.SubSeconds);
			}
		}
	}

	BaseType_t yield;
	xSemaphoreGiveFromISR(sem_nextsend_isready, &yield);
	if(yield) portYIELD_FROM_ISR(yield);
}

static void rejoin_to_network(void) {
	uplink_session_count = 0;
	LOGE(TAG, "*********************************** END **************************************");

	vTaskSuspend(htask_app);
	if (TimerIsStarted(&send_repeat_timer)) 	TimerStop(&send_repeat_timer);
	if (TimerIsStarted(&regularcheck_timer)) 	TimerStop(&regularcheck_timer);
	if (TimerIsStarted(&preactivation_timer)) 	TimerStop(&preactivation_timer);
	if (TimerIsStarted(&activation_timer)) 		TimerStop(&activation_timer);
	LmHandlerJoin();
}

//static void newactivationtime_apply_and_starttimer(uint32_t new_next_act) {
//	SysTime_t curTime = SysTimeGet();
//	uint32_t curr_time_ms = (curTime.Seconds * 1000U) + curTime.SubSeconds;
//	/**
//	 * cc: Phần if này chắc có bug đơn vị thời gian, uint32_t có đủ hay không?
//	 */
//	if (new_next_act > curr_time_ms
//			&& new_next_act != curr_activation_time){
//		curr_activation_time = new_next_act;
//		LOGE(TAG, "Preactivation will occur at %lums, activation will occur at %lums", curr_activation_time - PRE_TO_ACTIVATION_TIME, curr_activation_time);
//		TimerStop(&preactivation_timer);
//		TimerStop(&activation_timer);
//		TimerSetValue(&preactivation_timer, curr_activation_time - PRE_TO_ACTIVATION_TIME - curr_time_ms);
//		TimerSetValue(&activation_timer, curr_activation_time - curr_time_ms);
//		TimerStart(&preactivation_timer);
//		TimerStart(&activation_timer);
//		enable_activation = true;
//		max_send_count = DEFAUT_MAX_SEND_COUNT;
//	}
//}

static void newactivationtime_apply_and_starttimer(uint32_t new_next_act) {
	SysTime_t curTime = SysTimeGet();
	uint32_t curr_time_s = curTime.Seconds;

	LOGV(TAG, "Hiện tại là %lu, mới là %lu", curr_time_s, new_next_act);
	if (new_next_act > (curr_time_s + PRE_TO_ACTIVATION_TIME + PRE_ACTIVATION_DELAY) && new_next_act != curr_activation_time){
		curr_activation_time = new_next_act;
		LOGE(TAG, "Preactivation will occur at %lus, activation will occur at %lus", curr_activation_time - PRE_TO_ACTIVATION_TIME, curr_activation_time);
		TimerStop(&preactivation_timer);
		TimerStop(&activation_timer);
		TimerSetValue(&preactivation_timer, (curr_activation_time - PRE_TO_ACTIVATION_TIME - curr_time_s)*1000U);
		TimerSetValue(&activation_timer, (curr_activation_time - curr_time_s)*1000U);
		TimerStart(&preactivation_timer); TimerStart(&activation_timer);
		enable_activation = true; max_send_count = DEFAUT_MAX_SEND_COUNT;
	}
}


static void startsession_blockapp(appmsg_types_t session) {
	LOGE(TAG, "********************************** START *************************************");
	LOGV(TAG, "%s", SessionString[session]);
	send_uplink_message(session);

	TimerSetContext(&send_repeat_timer, &session);
	TimerSetValue(&send_repeat_timer, 200);
	TimerStart(&send_repeat_timer);

	vTaskSuspend(htask_app);
}

static void stopsession_releaseapp(void) {
	TimerStop(&send_repeat_timer);

	if(xSemaphoreGive(sem_newoperation_isready) != pdPASS)
		LOGE(TAG, "Semaphore give failed"); /** cc: Lâu lâu bug ở đây */

	uplink_session_count = 0;

	vTaskResume(htask_app);

	LOGV(TAG, "End session");
	LOGE(TAG, "*********************************** END **************************************");

	EnterSleepMode();
}

/**
 * **********************************************************************************************************************************************
 * LoRaWAN On event handlers part 2.
 */

static void Task_LmHandlerProcess(void *){
	uint32_t notify_val;
	while(1){
		xTaskNotifyWait(0, 0, &notify_val, portMAX_DELAY);
		LmHandlerProcess( );
		(void)notify_val;
	}
}

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
//	DisplayMacMcpsRequestUpdate(status, mcpsReq, nextTxIn);
}

static void OnMacMlmeRequest(LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn) {
	DisplayMacMlmeRequestUpdate(status, mlmeReq, nextTxIn);
}

static void OnTxPeriodicityChanged(uint32_t periodicity) {

}

static void OnTxFrameCtrlChanged(LmHandlerMsgTypes_t isTxConfirmed) {
	LmHandlerParams.IsTxConfirmed = isTxConfirmed;
}

static void OnPingSlotPeriodicityChanged(uint8_t pingSlotPeriodicity) {
	LmHandlerParams.PingSlotPeriodicity = pingSlotPeriodicity;
}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == LORA_INT_Pin) {
		RadioOnDioIrq();
	}
	if (GPIO_Pin == NFC_WAKE_Pin) {
		for(uint32_t i=0; i<120000; i++) __NOP(); //10ms

		if(HAL_GPIO_ReadPin(NFC_WAKE_GPIO_Port, NFC_WAKE_Pin) == GPIO_PIN_RESET){
			LOGV(TAG, "Start activation due to user press button");
			TimerStop(&activation_timer);
			EnableSleepMode(false);
			xTaskResumeFromISR(htask_activation);
			running_step = 0;
		}
	}
}





