/*
 * timer.c
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */


#include "stdlib.h"
#include "stm32l4xx_hal.h"
#include <util_timer/util_timer.h>
#include <util_timer/util_timer_drv.h>



static util_timer_t *headtimer = NULL;
extern const util_timer_driver_t UTIL_TimerDriver;

static void timer_insert_head(util_timer_t *utimer);
static void timer_insert(util_timer_t *utimer);
static void timer_set_timeout(util_timer_t *utimer);
static bool timer_exist(util_timer_t *utimer);

util_timer_status_t util_timer_init(void) {
	__disable_irq();
	headtimer = NULL;
	__enable_irq();

	return UTIL_TIMER_OK;
}

util_timer_status_t util_timer_create(util_timer_t *utimer, uint64_t period,
		util_timer_mode_t mode, void (*event_callback)(void*), void *argument) {
	if ((utimer != NULL) && (event_callback != NULL)) {
		utimer->timestamp = 0U;
		utimer->reload = period;
		utimer->ispending = 0U;
		utimer->isrunning = 0U;
		utimer->isreloadstopped = 0U;
		utimer->event_callback = event_callback;
		utimer->argument = argument;
		utimer->mode = mode;
		utimer->next = NULL;
		return UTIL_TIMER_OK;
	}

	return UTIL_TIMER_INVALID_PARAM;
}

util_timer_status_t util_timer_start(util_timer_t *utimer) {
	util_timer_status_t ret = UTIL_TIMER_OK;
	uint64_t elapsedTime;
	uint64_t minValue;
	uint64_t ticks;

	if ((utimer != NULL) && (timer_exist(utimer) == false)
			&& (utimer->isrunning == 0U)) {
		__disable_irq();
		ticks = utimer->reload;
		minValue = 1;

		if (ticks < minValue) {
			ticks = minValue;
		}

		utimer->timestamp = ticks;
		utimer->ispending = 0U;
		utimer->isrunning = 1U;
		utimer->isreloadstopped = 0U;
		if (headtimer == NULL) {
			UTIL_TimerDriver.SetTimerContext();
			timer_insert_head(utimer); /* insert a timeout at now+obj->timestamp */
		}
		else {
			elapsedTime = UTIL_TimerDriver.GetTimerElapsedTime();
			utimer->timestamp += elapsedTime;

			if (utimer->timestamp < headtimer->timestamp)
				timer_insert_head(utimer);
			else
				timer_insert(utimer);
		}
		__enable_irq();
	}
	else {
		ret = UTIL_TIMER_INVALID_PARAM;
	}

	return ret;
}

util_timer_status_t util_timer_start_period(util_timer_t *utimer,
		uint64_t period) {
	util_timer_status_t ret = UTIL_TIMER_OK;

	if (NULL == utimer)
		ret = UTIL_TIMER_INVALID_PARAM;
	else {
		utimer->reload = period;
		if (timer_exist(utimer)) {
			(void) util_timer_stop(utimer);
		}
		ret = util_timer_start(utimer);
	}

	return ret;
}

util_timer_status_t util_timer_stop(util_timer_t *utimer) {
	util_timer_status_t ret = UTIL_TIMER_OK;

	if (NULL != utimer) {
		__disable_irq();
		util_timer_t *prev = headtimer;
		util_timer_t *cur = headtimer;
		utimer->isreloadstopped = 1U;

		if (NULL != headtimer) {
			utimer->isrunning = 0U;

			if (headtimer == utimer) {
				headtimer->ispending = 0;
				if (headtimer->next != NULL) {
					headtimer = headtimer->next;
					timer_set_timeout(headtimer);
				}
				else {
					UTIL_TimerDriver.StopTimerEvt();
					headtimer = NULL;
				}
			}
			else {
				while (cur != NULL) {
					if (cur == utimer) {
						if (cur->next != NULL) {
							cur = cur->next;
							prev->next = cur;
						}
						else {
							cur = NULL;
							prev->next = cur;
						}
						break;
					}
					else {
						prev = cur;
						cur = cur->next;
					}
				}
			}
			ret = UTIL_TIMER_OK;
		}
		__enable_irq();
	}
	else
		ret = UTIL_TIMER_INVALID_PARAM;

	return ret;
}

util_timer_status_t util_timer_set_period(util_timer_t *utimer,
		uint64_t newperiod) {
	util_timer_status_t ret = UTIL_TIMER_OK;

	if (NULL == utimer) {
		ret = UTIL_TIMER_INVALID_PARAM;
	}
	else {
		utimer->reload = newperiod;
		if (timer_exist(utimer)) {
			(void) util_timer_stop(utimer);
			ret = util_timer_start(utimer);
		}
	}
	return ret;
}

util_timer_status_t util_timer_set_reloadmode(util_timer_t *utimer,
		util_timer_mode_t mode) {
	util_timer_status_t ret = UTIL_TIMER_OK;

	if (NULL == utimer)
		ret = UTIL_TIMER_INVALID_PARAM;
	else
		utimer->mode = mode;

	return ret;
}

util_timer_status_t util_timer_get_remainingtime(util_timer_t *utimer,
		uint64_t *elapsedtime) {
	util_timer_status_t ret = UTIL_TIMER_OK;
	if (timer_exist(utimer)) {
		uint64_t time = UTIL_TimerDriver.GetTimerElapsedTime();
		if (utimer->timestamp < time)
			*elapsedtime = 0;
		else
			*elapsedtime = utimer->timestamp - time;
	}
	else
		ret = UTIL_TIMER_INVALID_PARAM;

	return ret;
}

uint64_t util_timer_is_running(util_timer_t *utimer) {
	if (utimer != NULL) {
		return utimer->isrunning;
	}

	return 0;
}

uint64_t util_timer_get_firstremainingtime(void) {
	uint64_t NextTimer = UINT64_MAX;

	if (headtimer != NULL) {
		(void) util_timer_get_remainingtime(headtimer, &NextTimer);
	}

	return NextTimer;
}

uint64_t util_timer_get_currenttime(void) {
	return UTIL_TimerDriver.GetTimerValue();
}

uint64_t util_timer_get_elapsedtime(uint64_t past) {
	uint64_t nowInTicks = UTIL_TimerDriver.GetTimerValue();
	uint64_t pastInTicks = past;
	return nowInTicks - pastInTicks;
}

void util_timer_IRQ_handler(void) {
	util_timer_t *cur;
	uint32_t old, now, DeltaContext;

	__disable_irq();

	old = UTIL_TimerDriver.GetTimerContext();
	now = UTIL_TimerDriver.SetTimerContext();

	DeltaContext = now - old;

	if (headtimer != NULL) {
		cur = headtimer;
		do {
			if (cur->timestamp > DeltaContext)
				cur->timestamp -= DeltaContext;
			else
				cur->timestamp = 0;
			cur = cur->next;
		} while (cur != NULL);
	}

	/* Execute expired timer and update the list */
	while ((headtimer != NULL)
			&& ((headtimer->timestamp == 0U)
					|| (headtimer->timestamp < UTIL_TimerDriver.GetTimerElapsedTime()))) {
		cur = headtimer;
		headtimer = headtimer->next;
		cur->ispending = 0;
		cur->isrunning = 0;
		cur->event_callback(cur->argument);
		if ((cur->mode == UTIL_TIMER_PERIODIC)
				&& (cur->isreloadstopped == 0U)) {
			(void) util_timer_start(cur);
		}
	}

	if ((headtimer != NULL) && (headtimer->ispending == 0U))
		timer_set_timeout(headtimer);

	__enable_irq();
}

bool timer_exist(util_timer_t *utimer) {
	util_timer_t *cur = headtimer;

	while (cur != NULL) {
		if (cur == utimer) {
			return true;
		}
		cur = cur->next;
	}
	return false;
}

void timer_set_timeout(util_timer_t *utimer) {
	uint32_t minTicks = 1;
	utimer->ispending = 1;

	/* In case deadline too soon */
	if (utimer->timestamp
			< (UTIL_TimerDriver.GetTimerElapsedTime() + minTicks)) {
		utimer->timestamp = UTIL_TimerDriver.GetTimerElapsedTime() + minTicks;
	}
	UTIL_TimerDriver.StartTimerEvt(utimer->timestamp);
}

void timer_insert(util_timer_t *utimer) {
	util_timer_t *cur = headtimer;
	util_timer_t *next = headtimer->next;

	while (cur->next != NULL) {
		if (utimer->timestamp > next->timestamp) {
			cur = next;
			next = next->next;
		}
		else {
			cur->next = utimer;
			utimer->next = next;
			return;

		}
	}
	cur->next = utimer;
	utimer->next = NULL;
}

void timer_insert_head(util_timer_t *utimer) {
	util_timer_t *cur = headtimer;

	if (cur != NULL) {
		cur->ispending = 0;
	}

	utimer->next = cur;
	headtimer = utimer;
	timer_set_timeout(headtimer);
}
