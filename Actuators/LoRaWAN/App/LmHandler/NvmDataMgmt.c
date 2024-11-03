/*!
 * \file      NvmDataMgmt.c
 *
 * \brief     NVM context management implementation
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
 *              (C)2013-2017 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \author    Johannes Bruder ( STACKFORCE )
 */

#include <stdio.h>
#include "utilities.h"
#include "nvmm.h"
#include "LoRaMac.h"
#include "NvmDataMgmt.h"

/*!
 * Enables/Disables the context storage management storage.
 * Must be enabled for LoRaWAN 1.0.4 or later.
 */
#ifndef CONTEXT_MANAGEMENT_ENABLED
#define CONTEXT_MANAGEMENT_ENABLED         0
#endif

static uint16_t NvmNotifyFlags = 0;

void NvmDataMgmtEvent(uint16_t notifyFlags) {
	NvmNotifyFlags = notifyFlags;
}

uint16_t NvmDataMgmtStore(void) {
#if( CONTEXT_MANAGEMENT_ENABLED == 1 )
	uint16_t dataSize = 0;
	MibRequestConfirm_t mibReq;
	mibReq.Type = MIB_NVM_CTXS;
	LoRaMacMibGetRequestConfirm(&mibReq);
	LoRaMacNvmData_t *nvm = mibReq.Param.Contexts;

	// Input checks
	if (NvmNotifyFlags == LORAMAC_NVM_NOTIFY_FLAG_NONE) {
		return 0;
	}
	if (LoRaMacStop() != LORAMAC_STATUS_OK) {
		return 0;
	}
	if (FLASH_IF_Erase(LORAWAN_NVM_BASE_ADDRESS, FLASH_PAGE_SIZE) == FLASH_IF_OK) {
		dataSize = ( ( sizeof( LoRaMacNvmData_t ) + 7 ) & ~0x07 );
		FLASH_IF_Write(LORAWAN_NVM_BASE_ADDRESS, (const void*) nvm, dataSize);
	}

	NvmNotifyFlags = LORAMAC_NVM_NOTIFY_FLAG_NONE;
	// Resume LoRaMac
	LoRaMacStart();

	return dataSize;
#else
    return 0;
#endif
}

uint16_t NvmDataMgmtRestore(void) {
#if( CONTEXT_MANAGEMENT_ENABLED == 1 )
	MibRequestConfirm_t mibReq;
	mibReq.Type = MIB_NVM_CTXS;
	LoRaMacMibGetRequestConfirm(&mibReq);
	LoRaMacNvmData_t *nvm = mibReq.Param.Contexts;

	FLASH_IF_Read((void *)nvm, LORAWAN_NVM_BASE_ADDRESS, sizeof( LoRaMacNvmData_t ));

#endif
	return 0;
}

bool NvmDataMgmtFactoryReset(void) {
#if( CONTEXT_MANAGEMENT_ENABLED == 1 )
//	uint16_t offset = 0;
//	// Crypto
//	if (NvmmReset(sizeof(LoRaMacCryptoNvmData_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(LoRaMacCryptoNvmData_t);
//
//	// Mac Group 1
//	if (NvmmReset(sizeof(LoRaMacNvmDataGroup1_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(LoRaMacNvmDataGroup1_t);
//
//	// Mac Group 2
//	if (NvmmReset(sizeof(LoRaMacNvmDataGroup2_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(LoRaMacNvmDataGroup2_t);
//
//	// Secure element
//	if (NvmmReset(sizeof(SecureElementNvmData_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(SecureElementNvmData_t);
//
//	// Region group 1
//	if (NvmmReset(sizeof(RegionNvmDataGroup1_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(RegionNvmDataGroup1_t);
//
//	// Region group 2
//	if (NvmmReset(sizeof(RegionNvmDataGroup2_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(RegionNvmDataGroup2_t);
//
//	// Class b
//	if (NvmmReset(sizeof(LoRaMacClassBNvmData_t), offset) == false) {
//		return false;
//	}
//	offset += sizeof(LoRaMacClassBNvmData_t);
#endif
	return true;
}
