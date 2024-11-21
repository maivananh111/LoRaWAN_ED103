/*
 * lora_porting.c
 *
 *  Created on: Oct 20, 2024
 *      Author: anh
 */

#include "lora_porting.h"
#include "logger.h"
#include "timer.h"
#include "radio.h"
#include "adc_if.h"



static const char *TAG = "BOARD";
static TimerEvent_t timer_sleep;
static bool enable_sleep = true;



static void OnWakeUp(void *);



void app_enter_critical(void){
	__disable_irq();
}

void app_exit_critical(void){
	__enable_irq();
}





uint8_t GetBatteryLevel(void){
	float adc = GetBatteryVol()*1000;

	float param_a = 255/(4200-2800);
	float param_b = -2800*param_a;

	uint8_t level = (uint8_t)((adc * param_a) + param_b);

	return (level > 0)? level : 0;
}


float GetBatteryVol(void){
	uint16_t adc = ADC_ReadChannels(ADC_CHANNEL_15);

	return (float)((adc * 0.00026579805) + 3.179);
}



uint32_t GetRandomSeed(void){
	return ( ( *( uint32_t* )UID_BASE ) ^ ( *( uint32_t* )UID_BASE+1 ) ^ ( *( uint32_t* )UID_BASE+2 ) );
}

uint32_t getRandom(void){
	return Radio.Random();
}



void EnableSleepMode(bool ena){
	enable_sleep = ena;
}

void EnterSleepMode(void){
	if (!enable_sleep) return;

  	HAL_SuspendTick();
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	extern HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
	HAL_InitTick(15);
	HAL_ResumeTick();
	extern void SystemClock_Config(void);
	SystemClock_Config();
}

void EnterSleepModeOn(uint32_t time){
	if (!enable_sleep) return;

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
	LOGW(TAG, "Exit sleep mode after %lums, current time stamp is %lums", time, TimerGetCurrentTime());
}



static void OnWakeUp(void *){
	TimerStop(&timer_sleep);
}



