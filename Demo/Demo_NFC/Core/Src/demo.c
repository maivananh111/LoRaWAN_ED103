/******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 *
 * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        www.st.com/myliberty
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
 * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/*! \file
 *
 *  \author 
 *
 *  \brief Demo application
 *
 *  This demo shows how to poll for several types of NFC cards/devices and how 
 *  to exchange data with these devices, using the RFAL library.
 *
 *  This demo does not fully implement the activities according to the standards,
 *  it performs the required to communicate with a card/device and retrieve 
 *  its UID. Also blocking methods are used for data exchange which may lead to
 *  long periods of blocking CPU/MCU.
 *  For standard compliant example please refer to the Examples provided
 *  with the RFAL library.
 * 
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "demo.h"
#include "utils.h"
#include "rfal_nfc.h"
#include "rfal_nfca.h"
#include "Tracelog/TraceLog.h"

/*
 ******************************************************************************
 * GLOBAL DEFINES
 ******************************************************************************
 */

/* Definition of possible states the demo state machine could have */
#define DEMO_ST_NOTINIT               0     /*!< Demo State:  Not initialized        */
#define DEMO_ST_START_DISCOVERY       1     /*!< Demo State:  Start Discovery        */
#define DEMO_ST_DISCOVERY             2     /*!< Demo State:  Discovery              */

#define DEMO_NFCV_BLOCK_LEN           4     /*!< NFCV Block len                      */

#define DEMO_NFCV_USE_SELECT_MODE     false /*!< NFCV demonstrate select mode        */
#define DEMO_NFCV_WRITE_TAG           false /*!< NFCV demonstrate Write Single Block */

/*
 ******************************************************************************
 * GLOBAL MACROS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/* P2P communication data */
static uint8_t NFCID3[] = { 0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0A };
static uint8_t GB[] = { 0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07,
		0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03 };

/* APDUs communication data */
static uint8_t ndefSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76,
		0x00, 0x00, 0x85, 0x01, 0x01, 0x00 };
static uint8_t ccSelectFile[] = { 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x03 };
static uint8_t readBynary[] = { 0x00, 0xB0, 0x00, 0x00, 0x0F };
/*static uint8_t ppseSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };*/

/* P2P communication data */
static uint8_t ndefLLCPSYMM[] = { 0x00, 0x00 };
static uint8_t ndefInit[] = { 0x05, 0x20, 0x06, 0x0F, 0x75, 0x72, 0x6E, 0x3A,
		0x6E, 0x66, 0x63, 0x3A, 0x73, 0x6E, 0x3A, 0x73, 0x6E, 0x65, 0x70, 0x02,
		0x02, 0x07, 0x80, 0x05, 0x01, 0x02 };
static uint8_t ndefUriSTcom[] = { 0x13, 0x20, 0x00, 0x10, 0x02, 0x00, 0x00,
		0x00, 0x19, 0xc1, 0x01, 0x00, 0x00, 0x00, 0x12, 0x55, 0x00, 0x68, 0x74,
		0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x73, 0x74, 0x2e,
		0x63, 0x6f, 0x6d };

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
static rfalNfcDiscoverParam discParam;
static uint8_t state = DEMO_ST_NOTINIT;
#define MAX_HEX_STR         4
#define MAX_HEX_STR_LENGTH  128
char hexStr[MAX_HEX_STR][MAX_HEX_STR_LENGTH];
uint8_t hexStrIdx = 0;
static const char *TAG = "NFC";
/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */
char* hex2Str(unsigned char * data, size_t dataLen);

static void demoP2P(void);
static void demoAPDU(void);
static void demoNfca(rfalNfcaListenDevice *nfcaDev);
static void demoNfcv(rfalNfcvListenDevice *nfcvDev);
static void demoNfcf(rfalNfcfListenDevice *nfcfDev);
static void demoCE(rfalNfcDevice *nfcDev);
static void demoNotif(rfalNfcState st);
ReturnCode demoTransceiveBlocking(uint8_t *txBuf, uint16_t txBufSize,
		uint8_t **rxBuf, uint16_t **rcvLen, uint32_t fwt);


char* hex2Str(unsigned char *data, size_t dataLen) {
	unsigned char *pin = data;
	const char *hex = "0123456789ABCDEF";
	char *pout = hexStr[hexStrIdx];
	uint8_t i = 0;
	uint8_t idx = hexStrIdx;
	size_t len;

	if (dataLen == 0) {
		pout[0] = 0;
	}
	else {
		/* Trim data that doesn't fit in buffer */
		len = MIN(dataLen, (MAX_HEX_STR_LENGTH / 2));

		for (; i < (len - 1); ++i) {
			*pout++ = hex[(*pin >> 4) & 0xF];
			*pout++ = hex[(*pin++) & 0xF];
		}
		*pout++ = hex[(*pin >> 4) & 0xF];
		*pout++ = hex[(*pin) & 0xF];
		*pout = 0;
	}

	hexStrIdx++;
	hexStrIdx %= MAX_HEX_STR;

	return hexStr[idx];
}
/*!
 *****************************************************************************
 * \brief Demo Notification
 *
 *  This function receives the event notifications from RFAL
 *****************************************************************************
 */
static void demoNotif(rfalNfcState st) {
	uint8_t devCnt;
	rfalNfcDevice *dev;

	if (st == RFAL_NFC_STATE_WAKEUP_MODE) {
//		LOG_INFO(TAG, "NFC card detecting");
	}
	else if (st == RFAL_NFC_STATE_POLL_TECHDETECT) {
//		LOG_EVENT(TAG, "Detected.");
	}
	else if (st == RFAL_NFC_STATE_POLL_SELECT) {
		/* Multiple devices were found, activate first of them */
		rfalNfcGetDevicesFound(&dev, &devCnt);
		rfalNfcSelect(0);

		LOG_EVENT(TAG, "Multiple Tags detected: %d \r\n", devCnt);
	}
}

/*!
 *****************************************************************************
 * \brief Demo Ini
 *
 *  This function Initializes the required layers for the demo
 *
 * \return true  : Initialization ok
 * \return false : Initialization failed
 *****************************************************************************
 */
bool demoIni(void) {
	ReturnCode err;

	err = rfalNfcInitialize();
	if (err == ERR_NONE) {
		discParam.compMode = RFAL_COMPLIANCE_MODE_NFC;
		discParam.devLimit = 1U;
		discParam.nfcfBR = RFAL_BR_212;
		discParam.ap2pBR = RFAL_BR_424;

		ST_MEMCPY(&discParam.nfcid3, NFCID3, sizeof(NFCID3));
		ST_MEMCPY(&discParam.GB, GB, sizeof(GB));
		discParam.GBLen = sizeof(GB);

		discParam.notifyCb = demoNotif;
		discParam.totalDuration = 1000U;
		discParam.wakeupEnabled = true;
		discParam.wakeupConfigDefault = true;
		discParam.techs2Find = ( RFAL_NFC_POLL_TECH_A | RFAL_NFC_POLL_TECH_B
				| RFAL_NFC_POLL_TECH_F | RFAL_NFC_POLL_TECH_V
				| RFAL_NFC_POLL_TECH_ST25TB);
        discParam.techs2Find   |= RFAL_NFC_POLL_TECH_AP2P;

		state = DEMO_ST_START_DISCOVERY;
		return true;
	}
	return false;
}

/*!
 *****************************************************************************
 * \brief Demo Cycle
 *
 *  This function executes the demo state machine. 
 *  It must be called periodically
 *****************************************************************************
 */
int demoCycle(void) {
	int x = 0;
	static rfalNfcDevice *nfcDevice;

	rfalNfcWorker(); /* Run RFAL worker periodically */

	/*******************************************************************************/
	/* Check if USER button is pressed */
	if (platformGpioIsLow(PLATFORM_USER_BUTTON_PORT,
			PLATFORM_USER_BUTTON_PIN)) {
		discParam.wakeupEnabled = !discParam.wakeupEnabled; /* enable/disable wakeup */
		state = DEMO_ST_START_DISCOVERY; /* restart loop          */
		platformLog("Toggling Wake Up mode %s\r\n",
				discParam.wakeupEnabled ? "ON" : "OFF");

		/* Debounce button */
		while (platformGpioIsLow(PLATFORM_USER_BUTTON_PORT,
				PLATFORM_USER_BUTTON_PIN));
	}

	switch (state) {
		/*******************************************************************************/
		case DEMO_ST_START_DISCOVERY:

			rfalNfcDeactivate( false);
			rfalNfcDiscover(&discParam);

			state = DEMO_ST_DISCOVERY;
		break;

			/*******************************************************************************/
		case DEMO_ST_DISCOVERY:

			if (rfalNfcIsDevActivated(rfalNfcGetState())) {
				rfalNfcGetActiveDevice(&nfcDevice);

				switch (nfcDevice->type) {
					/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_NFCA:

						switch (nfcDevice->dev.nfca.type) {
							case RFAL_NFCA_T1T:
								LOG_WARN(TAG, "ISO14443A/Topaz (NFC-A T1T) TAG found. UID: %s",
										hex2Str(nfcDevice->nfcid,
												nfcDevice->nfcidLen));
							break;

							case RFAL_NFCA_T4T:
								LOG_WARN(TAG, "NFCA Passive ISO-DEP device found. UID: %s", hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));

								demoAPDU();
							break;

							case RFAL_NFCA_T4T_NFCDEP:
							case RFAL_NFCA_NFCDEP:
								LOG_WARN(TAG,
										"NFCA Passive P2P device found. NFCID: %s",
										hex2Str(nfcDevice->nfcid,
												nfcDevice->nfcidLen));

								demoP2P();
							break;

							default:
								LOG_WARN(TAG,
										"ISO14443A/NFC-A card found. UID: %s",
										hex2Str(nfcDevice->nfcid,
												nfcDevice->nfcidLen));
								demoNfca(&nfcDevice->dev.nfca);
							break;
						}
					break;

						/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_NFCB:
						LOG_WARN(TAG,"ISO14443B/NFC-B card found. UID: %s",
								hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));

						if (rfalNfcbIsIsoDepSupported(&nfcDevice->dev.nfcb)) {
//							demoAPDU();
						}
					break;

						/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_NFCF:

						if (rfalNfcfIsNfcDepSupported(&nfcDevice->dev.nfcf)) {
							LOG_WARN(TAG,
									"NFCF Passive P2P device found. NFCID: %s",
									hex2Str(nfcDevice->nfcid,
											nfcDevice->nfcidLen));
							demoP2P();
						}
						else {
							LOG_WARN(TAG, "Felica/NFC-F card found. UID: %s",
									hex2Str(nfcDevice->nfcid,
											nfcDevice->nfcidLen));

							demoNfcf(&nfcDevice->dev.nfcf);
						}

					break;

						/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_NFCV: {
						uint8_t devUID[RFAL_NFCV_UID_LEN];

						ST_MEMCPY(devUID, nfcDevice->nfcid,
								nfcDevice->nfcidLen); /* Copy the UID into local var */
						REVERSE_BYTES(devUID, RFAL_NFCV_UID_LEN); /* Reverse the UID for display purposes */
						LOG_WARN(TAG,"ISO15693/NFC-V card found. UID: %s",
								hex2Str(devUID, RFAL_NFCV_UID_LEN));


						demoNfcv(&nfcDevice->dev.nfcv);
					}
					break;

						/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_ST25TB:

						LOG_WARN(TAG,"ST25TB card found. UID: %s",
								hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
					break;

						/*******************************************************************************/
					case RFAL_NFC_LISTEN_TYPE_AP2P:

						LOG_WARN(TAG,
								"NFC Active P2P device found. NFCID3: %s",
								hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));

						demoP2P();
					break;

						/*******************************************************************************/
					case RFAL_NFC_POLL_TYPE_NFCA:
					case RFAL_NFC_POLL_TYPE_NFCF:

						LOG_WARN(TAG,"Activated in CE %s mode.",
								(nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ?
										"NFC-A" : "NFC-F");

						demoCE(nfcDevice);
					break;

						/*******************************************************************************/
					default:
					break;
				}

				rfalNfcDeactivate( false);
				platformDelay(50);
				state = DEMO_ST_START_DISCOVERY;
				x = 1;
			}
		break;

			/*******************************************************************************/
		case DEMO_ST_NOTINIT:
		default:
		break;
	}

	return x;
}

static void demoCE( rfalNfcDevice *nfcDev )
{
#if defined(ST25R3916) && defined(RFAL_FEATURE_LISTEN_MODE)

    ReturnCode err;
    uint8_t *rxData;
    uint16_t *rcvLen;
    uint8_t  txBuf[100];
    uint16_t txLen;

    demoCeInit( ceNFCF_nfcid2 );

    do
    {
        rfalNfcWorker();

        switch( rfalNfcGetState() )
        {
            case RFAL_NFC_STATE_ACTIVATED:
                err = demoTransceiveBlocking( NULL, 0, &rxData, &rcvLen, 0);
                break;

            case RFAL_NFC_STATE_DATAEXCHANGE:
            case RFAL_NFC_STATE_DATAEXCHANGE_DONE:

                txLen = ( (nfcDev->type == RFAL_NFC_POLL_TYPE_NFCA) ? demoCeT4T( rxData, *rcvLen, txBuf, sizeof(txBuf) ): demoCeT3T( rxData, *rcvLen, txBuf, sizeof(txBuf) ) );
                err   = demoTransceiveBlocking( txBuf, txLen, &rxData, &rcvLen, RFAL_FWT_NONE );
                break;

            case RFAL_NFC_STATE_LISTEN_SLEEP:
            default:
                break;
        }
    }
    while( (err == ERR_NONE) || (err == ERR_SLEEP_REQ) );

#endif /* RFAL_FEATURE_LISTEN_MODE */
}

/*!
 *****************************************************************************
 * \brief Demo NFC-F 
 *
 * Example how to exchange read and write blocks on a NFC-F tag
 * 
 *****************************************************************************
 */
static void demoNfcf(rfalNfcfListenDevice *nfcfDev) {
	ReturnCode err;
	uint8_t buf[(RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN
			+ (3 * RFAL_NFCF_BLOCK_LEN))];
	uint16_t rcvLen;
	rfalNfcfServ srv = RFAL_NFCF_SERVICECODE_RDWR;
	rfalNfcfBlockListElem bl[3];
	rfalNfcfServBlockListParam servBlock;
	//uint8_t                    wrData[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

	servBlock.numServ = 1; /* Only one Service to be used           */
	servBlock.servList = &srv; /* Service Code: NDEF is Read/Writeable  */
	servBlock.numBlock = 1; /* Only one block to be used             */
	servBlock.blockList = bl;
	bl[0].conf = RFAL_NFCF_BLOCKLISTELEM_LEN; /* Two-byte Block List Element           */
	bl[0].blockNum = 0x0001; /* Block: NDEF Data                      */

	err = rfalNfcfPollerCheck(nfcfDev->sensfRes.NFCID2, &servBlock, buf,
			sizeof(buf), &rcvLen);
	platformLog(" Check Block: %s Data:  %s \r\n",
			(err != ERR_NONE) ? "FAIL": "OK",
			(err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN));

#if 0  /* Writing example */
        err = rfalNfcfPollerUpdate( nfcfDev->sensfRes.NFCID2, &servBlock, buf , sizeof(buf), wrData, buf, sizeof(buf) );
        platformLog(" Update Block: %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( wrData, RFAL_NFCF_BLOCK_LEN) );
        err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
        platformLog(" Check Block:  %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
    #endif
}

static void demoNfca(rfalNfcaListenDevice *nfcaDev) {
//	ReturnCode err;
//	rfalNfcaListenDevice nfcaDevList;
//	rfalNfcaSelRes selres;
//    uint8_t nfcaDevCount = 0;
//
//    if (rfalNfcaPollerInitialize() != ERR_NONE) {
//    	LOG_ERROR(TAG, "Error NFC-A card initialize fail");
//        return;
//    }

//    if (rfalNfcaPollerFullCollisionResolution(RFAL_COMPLIANCE_MODE_NFC, 1, &nfcaDevList, &nfcaDevCount) != ERR_NONE) {
//    	LOG_ERROR(TAG, "Error NFC-A card PollerFullCollisionResolution fail");
//        return;
//    }
//
//    if (nfcaDevCount > 0) {
//    	LOG_ERROR(TAG, "Error NFC-A card count");
//    }
//
//	err = rfalNfcaPollerSelect(nfcaDev->nfcId1, nfcaDev->nfcId1Len, &nfcaDev->selRes);
//	if(err != ERR_NONE) LOG_ERROR(TAG, "Error NFC-A card poller select");
//	err = rfalNfcaPollerTechnologyDetection(RFAL_COMPLIANCE_MODE_NFC, &nfcaDev->sensRes);
//	if(err != ERR_NONE) LOG_ERROR(TAG, "Error NFC-A technology detection");
}
/*!
 *****************************************************************************
 * \brief Demo NFC-V Exchange
 *
 * Example how to exchange read and write blocks on a NFC-V tag
 * 
 *****************************************************************************
 */
static void demoNfcv(rfalNfcvListenDevice *nfcvDev) {
	ReturnCode err;
	uint16_t rcvLen;
	uint8_t blockNum = 1;
	uint8_t rxBuf[1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN]; /* Flags + Block Data + CRC */
	uint8_t *uid;
#if DEMO_NFCV_WRITE_TAG
    uint8_t               wrData[DEMO_NFCV_BLOCK_LEN] = { 0x11, 0x22, 0x33, 0x99 };             /* Write block example */
#endif

	uid = nfcvDev->InvRes.UID;

#if DEMO_NFCV_USE_SELECT_MODE
        /*
        * Activate selected state
        */
        err = rfalNfcvPollerSelect(RFAL_NFCV_REQ_FLAG_DEFAULT, nfcvDev->InvRes.UID );
        platformLog(" Select %s \r\n", (err != ERR_NONE) ? "FAIL (revert to addressed mode)": "OK" );
        if( err == ERR_NONE )
        {
            uid = NULL;
        }
    #endif    

	/*
	 * Read block using Read Single Block command
	 * with addressed mode (uid != NULL) or selected mode (uid == NULL)
	 */
	err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid,
			blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
	platformLog(" Read Block: %s %s\r\n",
			(err != ERR_NONE) ? "FAIL": "OK Data:",
			(err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));

#if DEMO_NFCV_WRITE_TAG /* Writing example */
        err = rfalNfcvPollerWriteSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, wrData, sizeof(wrData));
        platformLog(" Write Block: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( wrData, DEMO_NFCV_BLOCK_LEN) );
        err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
        platformLog(" Read Block: %s %s\r\n", (err != ERR_NONE) ? "FAIL": "OK Data:", (err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
    #endif
}

/*!
 *****************************************************************************
 * \brief Demo P2P Exchange
 *
 * Sends a NDEF URI record 'http://www.ST.com' via NFC-DEP (P2P) protocol.
 * 
 * This method sends a set of static predefined frames which tries to establish
 * a LLCP connection, followed by the NDEF record, and then keeps sending 
 * LLCP SYMM packets to maintain the connection.
 * 
 * 
 *****************************************************************************
 */
void demoP2P(void) {
	uint16_t *rxLen;
	uint8_t *rxData;
	ReturnCode err;

	platformLog(" Initalize device .. ");
	err = demoTransceiveBlocking(ndefInit, sizeof(ndefInit), &rxData, &rxLen,
			RFAL_FWT_NONE);
	if (err != ERR_NONE) {
		platformLog("failed.");
		return;
	}
	platformLog("succeeded.\r\n");

	platformLog(" Push NDEF Uri: www.ST.com .. ");
	err = demoTransceiveBlocking(ndefUriSTcom, sizeof(ndefUriSTcom), &rxData,
			&rxLen, RFAL_FWT_NONE);
	if (err != ERR_NONE) {
		platformLog("failed.");
		return;
	}
	platformLog("succeeded.\r\n");

	platformLog(" Device present, maintaining connection ");
	while (err == ERR_NONE) {
		err = demoTransceiveBlocking(ndefLLCPSYMM, sizeof(ndefLLCPSYMM),
				&rxData, &rxLen, RFAL_FWT_NONE);
		platformLog(".");
		platformDelay(50);
	}
	platformLog("\r\n Device removed.\r\n");
}

/*!
 *****************************************************************************
 * \brief Demo APDUs Exchange
 *
 * Example how to exchange a set of predefined APDUs with PICC. The NDEF
 * application will be selected and then CC will be selected and read.
 * 
 *****************************************************************************
 */
void demoAPDU(void) {
	ReturnCode err;
	uint16_t *rxLen;
	uint8_t *rxData;

	/* Exchange APDU: NDEF Tag Application Select command */
	err = demoTransceiveBlocking(ndefSelectApp, sizeof(ndefSelectApp), &rxData,
			&rxLen, RFAL_FWT_NONE);
	platformLog(" Select NDEF Application: %s Data: %s\r\n",
			(err != ERR_NONE) ? "FAIL": "OK", hex2Str(rxData, *rxLen));

	if ((err == ERR_NONE) && rxData[0] == 0x90 && rxData[1] == 0x00) {
		/* Exchange APDU: Select Capability Container File */
		err = demoTransceiveBlocking(ccSelectFile, sizeof(ccSelectFile),
				&rxData, &rxLen, RFAL_FWT_NONE);
		platformLog(" Select CC: %s Data: %s\r\n",
				(err != ERR_NONE) ? "FAIL": "OK", hex2Str(rxData, *rxLen));

		/* Exchange APDU: Read Capability Container File  */
		err = demoTransceiveBlocking(readBynary, sizeof(readBynary), &rxData,
				&rxLen, RFAL_FWT_NONE);
		platformLog(" Read CC: %s Data: %s\r\n",
				(err != ERR_NONE) ? "FAIL": "OK", hex2Str(rxData, *rxLen));
	}
//	else{
//		LOG_ERROR()
//	}
}

/*!
 *****************************************************************************
 * \brief Demo Blocking Transceive 
 *
 * Helper function to send data in a blocking manner via the rfalNfc module 
 *  
 * \warning A protocol transceive handles long timeouts (several seconds), 
 * transmission errors and retransmissions which may lead to a long period of 
 * time where the MCU/CPU is blocked in this method.
 * This is a demo implementation, for a non-blocking usage example please 
 * refer to the Examples available with RFAL
 *
 * \param[in]  txBuf      : data to be transmitted
 * \param[in]  txBufSize  : size of the data to be transmited
 * \param[out] rxData     : location where the received data has been placed
 * \param[out] rcvLen     : number of data bytes received
 * \param[in]  fwt        : FWT to be used (only for RF frame interface, 
 *                                          otherwise use RFAL_FWT_NONE)
 *
 * 
 *  \return ERR_PARAM     : Invalid parameters
 *  \return ERR_TIMEOUT   : Timeout error
 *  \return ERR_FRAMING   : Framing error detected
 *  \return ERR_PROTO     : Protocol error detected
 *  \return ERR_NONE      : No error, activation successful
 * 
 *****************************************************************************
 */
ReturnCode demoTransceiveBlocking(uint8_t *txBuf, uint16_t txBufSize,
		uint8_t **rxData, uint16_t **rcvLen, uint32_t fwt) {
	ReturnCode err;

	err = rfalNfcDataExchangeStart(txBuf, txBufSize, rxData, rcvLen, fwt);
	if (err == ERR_NONE) {
		do {
			rfalNfcWorker();
			err = rfalNfcDataExchangeGetStatus();
		}
		while (err == ERR_BUSY);
	}
	return err;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
