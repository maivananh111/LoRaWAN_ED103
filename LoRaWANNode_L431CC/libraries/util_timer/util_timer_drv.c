/*
 * util_timer_drv.c
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#include "stm32l4xx_hal.h"
#include "Tracelog/TraceLog.h"
#include <inttypes.h>
#include <util_timer/util_timer_drv.h>


extern LPTIM_HandleTypeDef hlptim1;
const util_timer_driver_t UTIL_TimerDriver ={
	util_timer_drv_starttimer,
	util_timer_drv_stoptimer,

	util_timer_drv_settimercontext,
	util_timer_drv_gettimercontext,

	util_timer_drv_gettimerelapsedtime,
	util_timer_drv_gettimervalue,
};


#ifndef UTIL_TIMER_IRQ_MAP_PROCESS
#define UTIL_TIMER_IRQ_MAP_PROCESS() util_timer_IRQ_handler()
#endif /* UTIL_TIMER_IRQ_MAP_PROCESS */

static __IO uint32_t timer_period_60s_n = 0;
static __IO uint16_t timer_period_remain = 0, timer_period = 0;
static __IO uint64_t tick_elapsed = 0;
static __IO uint64_t TimerContext = 0;





static inline uint64_t util_timer_drv_gettimerticks(void);

util_timer_status_t util_timer_drv_starttimer(uint64_t timeout) {
	if(timeout == 0) return UTIL_TIMER_INVALID_PARAM;

	timer_period_60s_n = timeout / 60000;
	timer_period_remain = timeout % 60000;
	if (timer_period_60s_n)
		timer_period = 60000;
	else
		timer_period = timer_period_remain;

	HAL_LPTIM_Counter_Stop_IT(&hlptim1);
//	LOG_WARN("TIMER", "Start timer, run %dms.", timer_period);
	return (util_timer_status_t) HAL_LPTIM_Counter_Start_IT(&hlptim1, timer_period);
}

util_timer_status_t util_timer_drv_stoptimer(void) {
	return (util_timer_status_t)HAL_LPTIM_Counter_Stop_IT(&hlptim1);
}

uint64_t util_timer_drv_settimercontext(void){
	TimerContext = util_timer_drv_gettimerticks();
	return TimerContext;
}

uint64_t util_timer_drv_gettimercontext(void) {
	return TimerContext;
}

uint64_t util_timer_drv_gettimerelapsedtime(void) {
	uint32_t ret = 0;
	ret = ((uint32_t) (util_timer_drv_gettimerticks() - TimerContext));
	return ret;
}

uint64_t util_timer_drv_gettimervalue(void) {
	return util_timer_drv_gettimerticks();
}

void util_timer_drv_delayms(uint64_t delay){
	if(delay > UINT16_MAX) delay = UINT16_MAX;

	uint64_t timeout = util_timer_drv_gettimerticks();
	while (((util_timer_drv_gettimerticks() - timeout)) < delay) {
		__NOP();
	}
}

uint64_t util_timer_drv_gettime(uint16_t *subSeconds) {
	uint64_t ticks =util_timer_drv_gettimerticks();
	uint64_t seconds = (uint64_t)(ticks/1000);
	*subSeconds = (uint32_t)(ticks%1000);

	return seconds;
}

static inline uint64_t util_timer_drv_gettimerticks(void) {
	return  tick_elapsed += hlptim1.Instance->CNT;
}


void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim){
	tick_elapsed += timer_period;
	/**
	 * timer_period_60s_n > 1 && timer_period_remain = 0 [2 - 0] V
	 * timer_period_60s_n = 1 && timer_period_remain = 0 [1 - 0] V
	 * timer_period_60s_n > 1 && timer_period_remain > 0 [2 - 1]
	 * timer_period_60s_n = 1 && timer_period_remain > 0 [1 - 1]
	 * timer_period_60s_n = 0 && timer_period_remain > 0 [0 - 1]
	 */
	if(timer_period_remain > 0 && timer_period_60s_n == 1){
		timer_period_60s_n--;
		timer_period = timer_period_remain;
		timer_period_remain = 0;
		HAL_LPTIM_Counter_Stop_IT(&hlptim1);
		HAL_LPTIM_Counter_Start_IT(&hlptim1, timer_period);

		return;
	}
	if(timer_period_60s_n > 0) {
		timer_period_60s_n--;
	}
	if(timer_period_60s_n == 0)	{
		HAL_LPTIM_Counter_Stop_IT(&hlptim1);
		UTIL_TIMER_IRQ_MAP_PROCESS();
//		LOG_WARN("TIMER", "End timer, event callback, tick_elapsed = %" PRIu64, tick_elapsed);

	}
}

