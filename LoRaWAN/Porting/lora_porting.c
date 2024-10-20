/*
 * lora_porting.c
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#include "lora_porting.h"
#include "logger.h"
#include "timer.h"



static const char *TAG = "BOARD";
static TimerEvent_t timer_sleep;

static void OnWakeUp(void *);




uint8_t GetBatteryLevel(void){
	return 127;
}

uint32_t GetRandomSeed(void){
	return ( ( *( uint32_t* )UID_BASE ) ^ ( *( uint32_t* )UID_BASE+1 ) ^ ( *( uint32_t* )UID_BASE+2 ) );
}

void EnterSleepMode(uint32_t time){
	TimerInit(&timer_sleep, OnWakeUp);
	TimerSetValue(&timer_sleep, time);
	TimerStart(&timer_sleep);

	LOGW(TAG, "Enter sleep mode on %lums, current time stamp is %lums", time, TimerGetCurrentTime());
  	HAL_SuspendTick();
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	extern HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
	HAL_InitTick(15);
	HAL_ResumeTick();
	extern void SystemClock_Config(void);
	SystemClock_Config();
	LOGW(TAG, "Exit sleep mode on %lums, current time stamp is %lums", time, TimerGetCurrentTime());
}

static void OnWakeUp(void *){
	TimerStop(&timer_sleep);
}



