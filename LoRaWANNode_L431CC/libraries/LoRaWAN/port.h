/*
 * port.h
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#ifndef LORAWAN_PORT_H_
#define LORAWAN_PORT_H_

#include "stm32l4xx_hal.h"
#include "util_timer/util_timer.h"
#include "Tracelog/TraceLog.h"
#include "spi.h"




#define RADIO_PHY_HSPI hspi1



#ifndef LIB_DEBUG
#define LIB_DEBUG 0
#endif

#if LIB_DEBUG > 0
#define LOG_LIB(tag, ...)                \
	do                                   \
	{                                    \
		LOG_INFO(tag, __VA_ARGS__);     \
	} while (0)
#else
#define LOG_LIB(...)
#endif




#define TimerTime_t uint64_t
#define TimerEvent_t util_timer_t
#define TIMERTIME_T_MAX ( ( uint32_t )~0 )

#define TimerInit(HANDLE, CB) do {\
                                   util_timer_create( HANDLE, TIMERTIME_T_MAX, UTIL_TIMER_ONESHOT, CB, NULL);\
                                 } while(0)

#define TimerSetValue(HANDLE, TIMEOUT) do{ \
                                           util_timer_set_period(HANDLE, TIMEOUT);\
                                         } while(0)
#define TimerStart(HANDLE)   do {\
                                  util_timer_start(HANDLE);\
                                } while(0)
#define TimerStop(HANDLE)   do {\
                                 util_timer_stop(HANDLE);\
                               } while(0)
#define TimerGetCurrentTime  util_timer_get_currenttime
#define TimerGetElapsedTime util_timer_get_elapsedtime







uint32_t BoardGetRandomSeed(void);

void BoardGetUniqueId(uint8_t *id);

uint8_t BoardGetBatteryLevel(void);

void BoardDisableIrq(void);

void BoardEnableIrq(void);

#endif /* LORAWAN_PORT_H_ */
