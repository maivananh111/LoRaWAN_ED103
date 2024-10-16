/*
 * util_timer.h
 *
 *  Created on: Jul 23, 2024
 *      Author: anh
 */

#ifndef UTIL_TIMER_UTIL_TIMER_H_
#define UTIL_TIMER_UTIL_TIMER_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"





typedef enum {
  UTIL_TIMER_ONESHOT  = 0,
  UTIL_TIMER_PERIODIC = 1
} util_timer_mode_t;

typedef enum {
  UTIL_TIMER_OK            = 0,
  UTIL_TIMER_INVALID_PARAM = 1,
  UTIL_TIMER_HW_ERROR      = 2,
  UTIL_TIMER_UNKNOWN_ERROR = 3
} util_timer_status_t;


typedef struct util_timer_s{
	uint64_t timestamp;
    uint64_t reload;
    uint8_t ispending;
    uint8_t isrunning;
    uint8_t isreloadstopped;
    util_timer_mode_t mode;
    void (*event_callback)(void *);
    void *argument;
	struct util_timer_s *next;
} util_timer_t;


util_timer_status_t util_timer_init(void);
util_timer_status_t util_timer_create(util_timer_t *utimer, uint64_t period, util_timer_mode_t mode, void (*event_callback)(void *), void *argument);
util_timer_status_t util_timer_start(util_timer_t *utimer);
util_timer_status_t util_timer_start_period(util_timer_t *utimer, uint64_t period);
util_timer_status_t util_timer_stop(util_timer_t *utimer );

util_timer_status_t util_timer_set_period(util_timer_t *utimer, uint64_t newperiod);
util_timer_status_t util_timer_set_reloadmode(util_timer_t *utimer, util_timer_mode_t mode);
util_timer_status_t util_timer_get_remainingtime(util_timer_t *utimer, uint64_t *elapsedtime);

uint64_t util_timer_is_running(util_timer_t *utimer);
uint64_t util_timer_get_firstremainingtime(void);
uint64_t util_timer_get_currenttime(void);
uint64_t util_timer_get_elapsedtime(uint64_t past);

void util_timer_IRQ_handler(void);



#ifdef __cplusplus
}
#endif


#endif /* UTIL_TIMER_UTIL_TIMER_H_ */
