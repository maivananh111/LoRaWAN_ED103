/*
 * util_timer_drv.h
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#ifndef UTIL_TIMER_UTIL_TIMER_DRV_H_
#define UTIL_TIMER_UTIL_TIMER_DRV_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <util_timer/util_timer.h>

typedef struct {
	util_timer_status_t (*StartTimerEvt)(uint64_t timeout);
	util_timer_status_t (*StopTimerEvt)(void);

	uint64_t (*SetTimerContext)(void);
	uint64_t (*GetTimerContext)(void);

	uint64_t (*GetTimerElapsedTime)(void);
	uint64_t (*GetTimerValue)(void);
} util_timer_driver_t;



util_timer_status_t util_timer_drv_init(void);

util_timer_status_t util_timer_drv_starttimer(uint64_t timeout);
util_timer_status_t util_timer_drv_stoptimer(void);

uint64_t util_timer_drv_gettime(uint16_t *subSeconds);
uint64_t util_timer_drv_gettimervalue(void);

uint64_t util_timer_drv_settimercontext(void);
uint64_t util_timer_drv_gettimercontext(void);

uint64_t util_timer_drv_gettimerelapsedtime(void);

void util_timer_drv_delayms(uint64_t delay);


#ifdef __cplusplus
}
#endif

#endif /* UTIL_TIMER_UTIL_TIMER_DRV_H_ */
