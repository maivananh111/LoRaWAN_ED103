/*!
 * \file      LmHandlerMsgDisplay.h
 *
 * \brief     Common set of functions to display default messages from
 *            LoRaMacHandler.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2019 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "string.h"
#include "utilities.h"
#include "timer.h"
#include "logger.h"
#include "LmHandlerMsgDisplay.h"



static const char *TAG = "LoRaWAN";
/*!
 * MAC status strings
 */
const char* MacStatusStrings[] ={
    "OK",                            // LORAMAC_STATUS_OK
    "Busy",                          // LORAMAC_STATUS_BUSY
    "Service unknown",               // LORAMAC_STATUS_SERVICE_UNKNOWN
    "Parameter invalid",             // LORAMAC_STATUS_PARAMETER_INVALID
    "Frequency invalid",             // LORAMAC_STATUS_FREQUENCY_INVALID
    "Datarate invalid",              // LORAMAC_STATUS_DATARATE_INVALID
    "Frequency or datarate invalid", // LORAMAC_STATUS_FREQ_AND_DR_INVALID
    "No network joined",             // LORAMAC_STATUS_NO_NETWORK_JOINED
    "Length error",                  // LORAMAC_STATUS_LENGTH_ERROR
    "Region not supported",          // LORAMAC_STATUS_REGION_NOT_SUPPORTED
    "Skipped APP data",              // LORAMAC_STATUS_SKIPPED_APP_DATA
    "Duty-cycle restricted",         // LORAMAC_STATUS_DUTYCYCLE_RESTRICTED
    "No channel found",              // LORAMAC_STATUS_NO_CHANNEL_FOUND
    "No free channel found",         // LORAMAC_STATUS_NO_FREE_CHANNEL_FOUND
    "Busy beacon reserved time",     // LORAMAC_STATUS_BUSY_BEACON_RESERVED_TIME
    "Busy ping-slot window time",    // LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME
    "Busy uplink collision",         // LORAMAC_STATUS_BUSY_UPLINK_COLLISION
    "Crypto error",                  // LORAMAC_STATUS_CRYPTO_ERROR
    "FCnt handler error",            // LORAMAC_STATUS_FCNT_HANDLER_ERROR
    "MAC command error",             // LORAMAC_STATUS_MAC_COMMAD_ERROR
    "ClassB error",                  // LORAMAC_STATUS_CLASS_B_ERROR
    "Confirm queue error",           // LORAMAC_STATUS_CONFIRM_QUEUE_ERROR
    "Multicast group undefined",     // LORAMAC_STATUS_MC_GROUP_UNDEFINED
    "Unknown error",                 // LORAMAC_STATUS_ERROR
};

/*!
 * MAC event info status strings.
 */
const char* EventInfoStatusStrings[] ={
    "OK",                            // LORAMAC_EVENT_INFO_STATUS_OK
    "Error",                         // LORAMAC_EVENT_INFO_STATUS_ERROR
    "Tx timeout",                    // LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT
    "Rx 1 timeout",                  // LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT
    "Rx 2 timeout",                  // LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT
    "Rx1 error",                     // LORAMAC_EVENT_INFO_STATUS_RX1_ERROR
    "Rx2 error",                     // LORAMAC_EVENT_INFO_STATUS_RX2_ERROR
    "Join failed",                   // LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL
    "Downlink repeated",             // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED
    "Tx DR payload size error",      // LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR
    "Downlink too many frames loss", // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS
    "Address fail",                  // LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL
    "MIC fail",                      // LORAMAC_EVENT_INFO_STATUS_MIC_FAIL
    "Multicast fail",                // LORAMAC_EVENT_INFO_STATUS_MULTICAST_FAIL
    "Beacon locked",                 // LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED
    "Beacon lost",                   // LORAMAC_EVENT_INFO_STATUS_BEACON_LOST
    "Beacon not found"               // LORAMAC_EVENT_INFO_STATUS_BEACON_NOT_FOUND
};

/*!
 * Prints the provided buffer in HEX
 * 
 * \param buffer Buffer to be printed
 * \param size   Buffer size to be printed
 */
char *ToHexBuffer(uint8_t *buffer, uint8_t size) {
	static char buf[770] = {0};

	memset(buf, 0, sizeof(buf));
	char *ptr = (char *)buf;
	for(uint8_t i=0; i<size-1; i++){
		snprintf(ptr, 4, "%02x,", buffer[i]);
		ptr += 3;
	}

	snprintf(ptr, 3, "%02x", buffer[size-1]);

	return buf;
}

void DisplayNvmDataChange(LmHandlerNvmContextStates_t state, uint16_t size) {
	if (state == LORAMAC_HANDLER_NVM_STORE)
		LOGI(TAG, "CONTEXTS STORED, Size: %i", size);
	else
		LOGI(TAG, "CONTEXTS RESTORED, Size: %i", size);
}

void DisplayNetworkParametersUpdate(CommissioningParams_t *commissioningParams) {
	LOGI(TAG, "DEVEUI : %s", ToHexBuffer(commissioningParams->DevEui, 8));
	LOGI(TAG, "JOINEUI: %s", ToHexBuffer(commissioningParams->JoinEui, 8));
}

void DisplayMacMcpsRequestUpdate(LoRaMacStatus_t status, McpsReq_t *mcpsReq, TimerTime_t nextTxIn) {
	switch (mcpsReq->Type) {
		case MCPS_CONFIRMED:
			LOGI(TAG, "MCPS CONFIRMED: %s", MacStatusStrings[status]);
			break;
		case MCPS_UNCONFIRMED:
			LOGI(TAG, "MCPS UNCONFIRMED: %s", MacStatusStrings[status]);
			break;
		case MCPS_PROPRIETARY:
			LOGI(TAG, "MCPS PROPRIETARY: %s", MacStatusStrings[status]);
			break;
		default:
			LOGW(TAG, "MCPS ERROR: %s", MacStatusStrings[status]);
			break;
	}
	if (status == LORAMAC_STATUS_DUTYCYCLE_RESTRICTED) LOGI(TAG, "Next Tx: %lu [ms]", nextTxIn);
}

void DisplayMacMlmeRequestUpdate(LoRaMacStatus_t status, MlmeReq_t *mlmeReq, TimerTime_t nextTxIn) {
	switch (mlmeReq->Type) {
		case MLME_JOIN:
			LOGW(TAG, "MAC REQUEST JOIN: %s", MacStatusStrings[status]);
			break;
		case MLME_LINK_CHECK:
			LOGW(TAG, "MAC REQUEST LINK_CHECK: %s", MacStatusStrings[status]);
			break;
		case MLME_DEVICE_TIME:
			LOGW(TAG, "MAC REQUEST DEVICE_TIME: %s", MacStatusStrings[status]);;
			break;
		case MLME_TXCW:
			LOGW(TAG, "MAC REQUEST TXCW: %s", MacStatusStrings[status]);
			break;
		default:
			LOGW(TAG, "MAC REQUEST UNKNOWN: %s", MacStatusStrings[status]);
			break;
	}
}

void DisplayJoinRequestUpdate(LmHandlerJoinParams_t *params) {
	if (params->Status == LORAMAC_HANDLER_SUCCESS) {
		LOGI(TAG, "NETWORK JOIN STATE: JOINED");
		if (params->CommissioningParams->IsOtaaActivation == true)
			LOGI(TAG, "ACTIVATION: OTAA");
		else
			LOGI(TAG, "ACTIVATION: ABP");
		LOGI(TAG, "DEVADDRESS: %08lx", params->CommissioningParams->DevAddr);
	}
	else
		LOGE(TAG, "NETWORK JOIN STATE: FAIL");
}

void DisplayTxUpdate(LmHandlerTxParams_t *params) {
	if (params->AppData.BufferSize != 0)
		LOGI(TAG, "UPLINK FRAME %lu: %s ", params->UplinkCounter, ToHexBuffer(params->AppData.Buffer, params->AppData.BufferSize));
}

void DisplayRxUpdate(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params) {
	if (appData->BufferSize != 0)
		LOGI(TAG, "DOWNLINK FRAME %lu: %s", params->DownlinkCounter, ToHexBuffer(appData->Buffer, appData->BufferSize));
}


void DisplayClassUpdate(DeviceClass_t deviceClass) {
	LOGI(TAG, "Switch to Class %c done.", "ABC"[deviceClass]);
}

void DisplayAppInfo(const char *appName, const Version_t *appVersion) {
	LOGI(TAG, "###### ===================================== ######");
	LOGI(TAG, "Application name   : %s", appName);
	LOGI(TAG, "Application version: %d.%d.%d", appVersion->Fields.Major,
			appVersion->Fields.Minor, appVersion->Fields.Patch);
	LOGI(TAG, "###### ===================================== ######");
}
