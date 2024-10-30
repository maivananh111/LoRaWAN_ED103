/*!
 * \file      soft-se-hal.h
 *
 * \brief     Secure Element hardware abstraction layer implementation
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
 *              (C)2020 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 */
//#include "board.h"
#include "radio.h"
#include "stm32l4xx_hal.h"
#include "soft-se-hal.h"

void SoftSeHalGetUniqueId( uint8_t *id )
{
    id[7] = ( ( *( uint32_t* )UID_BASE )+ ( *( uint32_t* )UID_BASE+2 ) ) >> 24;
    id[6] = ( ( *( uint32_t* )UID_BASE )+ ( *( uint32_t* )UID_BASE+2 ) ) >> 16;
    id[5] = ( ( *( uint32_t* )UID_BASE )+ ( *( uint32_t* )UID_BASE+2 ) ) >> 8;
    id[4] = ( ( *( uint32_t* )UID_BASE )+ ( *( uint32_t* )UID_BASE+2 ) );
    id[3] = ( ( *( uint32_t* )UID_BASE+1 ) ) >> 24;
    id[2] = ( ( *( uint32_t* )UID_BASE+1 ) ) >> 16;
    id[1] = ( ( *( uint32_t* )UID_BASE+1 ) ) >> 8;
    id[0] = ( ( *( uint32_t* )UID_BASE+1 ) );
}

uint32_t SoftSeHalGetRandomNumber( void )
{
    return Radio.Random( );
}
