/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    timer_if.c
  * @author  MCD Application Team
  * @brief   Configure RTC Alarm, Tick and Calendar manager
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "timer_if.h"
#include "main.h" /*for STM32CubeMX generated RTC_N_PREDIV_S and RTC_N_PREDIV_A*/
#include "stm32l4xx_hal.h"
#include "time.h"
//#include "utilities_conf.h"

/**
  * @brief RTC handle
  */
extern RTC_HandleTypeDef hrtc;

/**
  * @brief Timer driver callbacks handler
  */
const UTIL_TIMER_Driver_s UTIL_TimerDriver = {
	TIMER_IF_Init,
	NULL,

	TIMER_IF_StartTimer,
	TIMER_IF_StopTimer,

	TIMER_IF_SetTimerContext,
	TIMER_IF_GetTimerContext,

	TIMER_IF_GetTimerElapsedTime,
	TIMER_IF_GetTimerValue,
	TIMER_IF_GetMinimumTimeout,

	TIMER_IF_Convert_ms2Tick,
	TIMER_IF_Convert_Tick2ms,
};

/**
  * @brief SysTime driver callbacks handler
  */
const UTIL_SYSTIM_Driver_s UTIL_SYSTIMDriver = {
	TIMER_IF_BkUp_Write_Seconds,
	TIMER_IF_BkUp_Read_Seconds,
	TIMER_IF_BkUp_Write_SubSeconds,
	TIMER_IF_BkUp_Read_SubSeconds,
	TIMER_IF_GetTime,
};

/**
  * @brief Minimum timeout delay of Alarm in ticks
  */
#define MIN_ALARM_DELAY    1

/**
  * @brief Backup seconds register
  */
#define RTC_BKP_SECONDS    RTC_BKP_DR0

/**
  * @brief Backup subseconds register
  */
#define RTC_BKP_SUBSECONDS RTC_BKP_DR1

/**
  * @brief Backup msbticks register
  */
#define RTC_BKP_MSBTICKS   RTC_BKP_DR2

/* #define RTIF_DEBUG */

/**
  * @brief Map UTIL_TIMER_IRQ can be overridden in utilities_conf.h to Map on Task rather then Isr
  */
#ifndef UTIL_TIMER_IRQ_MAP_INIT
#define UTIL_TIMER_IRQ_MAP_INIT()
#endif /* UTIL_TIMER_IRQ_MAP_INIT */

#ifndef UTIL_TIMER_IRQ_MAP_PROCESS
#define UTIL_TIMER_IRQ_MAP_PROCESS() UTIL_TIMER_IRQ_Handler()
#endif /* UTIL_TIMER_IRQ_MAP_PROCESS */

#define TIMER_IF_DBG_PRINTF(...)
/**
  * @brief Indicates if the RTC is already Initialized or not
  */
static bool RTC_Initialized = false;

/**
  * @brief RtcTimerContext
  */
static uint64_t RtcTimerContext = 0;

/**
  * @brief Get rtc timer Value in rtc tick
  * @return val the rtc timer value (upcounting)
  */
static inline uint64_t GetTimerTicks(void);

/**
  * @brief Writes MSBticks to backup register
  * Absolute RTC time in tick is (MSBticks)<<32 + (32bits binary counter)
  * @note MSBticks incremented every time the 32bits RTC timer wraps around (~44days)
  * @param[in] MSBticks
  */
static void TIMER_IF_BkUp_Write_MSBticks(uint32_t MSBticks);

/**
  * @brief Reads MSBticks from backup register
  * Absolute RTC time in tick is (MSBticks)<<32 + (32bits binary counter)
  * @note MSBticks incremented every time the 32bits RTC timer wraps around (~44days)
  * @retval MSBticks
  */
static uint32_t TIMER_IF_BkUp_Read_MSBticks(void);





static uint64_t datetime_to_milisec(int year, int month, int day, int hours, int minutes, int seconds, uint32_t subseconds) {
    struct tm time_info = {0};
    time_info.tm_year = year;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = day;
    time_info.tm_hour = hours;
    time_info.tm_min = minutes;
    time_info.tm_sec = seconds;

    struct tm time_info_ref = {
		.tm_year = 0,
		.tm_mon = 1-1,
		.tm_mday = 1,
		.tm_hour = 0,
		.tm_min = 0,
		.tm_sec = 0,
    };

    time_t time_in_seconds = mktime(&time_info) - mktime(&time_info_ref);
    uint64_t time_in_milisec = ((uint64_t)time_in_seconds * 1000) + (1000 - subseconds);

    return time_in_milisec;
}

static void seconds_to_datetime(uint32_t Seconds, uint32_t Subseconds, RTC_DateTypeDef *pDate, RTC_TimeTypeDef *pTime) {
	struct tm *time_info = gmtime((time_t *)&Seconds);

    pDate->Date = time_info->tm_mday;
    pDate->Month = time_info->tm_mon + 1;
    pDate->Year = time_info->tm_year - 70;
    pTime->Hours = time_info->tm_hour;
    pTime->Minutes = time_info->tm_min;
    pTime->Seconds = time_info->tm_sec;
    pTime->SubSeconds = 1000 - Subseconds;
}

static void subseconds_to_datetime(uint64_t Subseconds, RTC_DateTypeDef *pDate, RTC_TimeTypeDef *pTime) {
	uint32_t _sub = Subseconds%1000;
	time_t _sec = (time_t)(Subseconds/1000);
	struct tm *time_info = gmtime(&_sec);

    pDate->Date = time_info->tm_mday;
    pDate->Month = time_info->tm_mon + 1;
    pDate->Year = time_info->tm_year - 70;
    pTime->Hours = time_info->tm_hour;
    pTime->Minutes = time_info->tm_min;
    pTime->Seconds = time_info->tm_sec;
    pTime->SubSeconds = 1000 - _sub;
}





/* Exported functions ---------------------------------------------------------*/
UTIL_TIMER_Status_t TIMER_IF_Init(void) {
	UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;
	/* USER CODE BEGIN TIMER_IF_Init */

	/* USER CODE END TIMER_IF_Init */
	if (RTC_Initialized == false) {
		/*Init RTC*/
		extern void MX_RTC_Init();
		MX_RTC_Init();
		/*Stop Timer */
		TIMER_IF_StopTimer();
		/*DeActivate the Alarm A enabled by STM32CubeMX during MX_RTC_Init() */
		HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
		/*Enable Direct Read of the calendar registers (not through Shadow) */
		HAL_RTCEx_EnableBypassShadow(&hrtc);
		/*Initialize MSB ticks*/
		TIMER_IF_BkUp_Write_MSBticks(0);
		TIMER_IF_SetTimerContext();
		/* Register a task to associate to UTIL_TIMER_Irq() interrupt */
		UTIL_TIMER_IRQ_MAP_INIT();

		RTC_Initialized = true;
	}

	return ret;
}

UTIL_TIMER_Status_t TIMER_IF_StartTimer(uint64_t timeout) {
	UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;
	RTC_AlarmTypeDef sAlarm = {0};
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	/*Stop timer if one is already started*/
	TIMER_IF_StopTimer();
	timeout += RtcTimerContext;
	subseconds_to_datetime(timeout, &sDate, &sTime);

	/* starts timer*/
	sAlarm.AlarmTime.Hours = sTime.Hours;
	sAlarm.AlarmTime.Minutes = sTime.Minutes;
	sAlarm.AlarmTime.Seconds = sTime.Seconds;
	sAlarm.AlarmTime.SubSeconds = sTime.SubSeconds;
	sAlarm.AlarmDateWeekDay = sDate.Date;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	return ret;
}

UTIL_TIMER_Status_t TIMER_IF_StopTimer(void) {
	UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;

	/* Clear RTC Alarm Flag */
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
	/* Disable the Alarm A interrupt */
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	return ret;
}

uint64_t TIMER_IF_SetTimerContext(void) {
	/*store time context*/
	RtcTimerContext = GetTimerTicks();
	/*return time context*/
	return RtcTimerContext;
}

uint64_t TIMER_IF_GetTimerContext(void) {
	return RtcTimerContext;
}

uint64_t TIMER_IF_GetTimerElapsedTime(void) {
	uint64_t ret = 0;
	ret = ((uint64_t) (GetTimerTicks() - RtcTimerContext));

	return ret;
}

uint64_t TIMER_IF_GetTimerValue(void) {
	uint64_t ret = 0;

	if (RTC_Initialized == true)
		ret = GetTimerTicks();

	return ret;
}

uint64_t TIMER_IF_GetMinimumTimeout(void) {
	uint64_t ret = 0;
	ret = (MIN_ALARM_DELAY);

	return ret;
}

uint64_t TIMER_IF_Convert_ms2Tick(uint64_t timeMilliSec) {
	return timeMilliSec;
}

uint64_t TIMER_IF_Convert_Tick2ms(uint64_t tick) {
	return tick;
}

void TIMER_IF_DelayMs(uint64_t delay) {
	uint64_t delayTicks = TIMER_IF_Convert_ms2Tick(delay);
	uint64_t timeout = GetTimerTicks();

	/* Wait delay ms */
	while (((GetTimerTicks() - timeout)) < delayTicks)
		__NOP();
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	UTIL_TIMER_IRQ_MAP_PROCESS();
}

/*
void HAL_RTCEx_SSRUEventCallback(RTC_HandleTypeDef *hrtc) {
	uint32_t MSB_ticks = TIMER_IF_BkUp_Read_MSBticks();
	TIMER_IF_BkUp_Write_MSBticks(MSB_ticks + 1);
}
*/

uint64_t TIMER_IF_GetTime(uint16_t *mSeconds) {
	uint32_t seconds = 0;

	uint64_t ticks = GetTimerTicks();
	seconds = ticks/1000;
	*mSeconds = TIMER_IF_Convert_Tick2ms(ticks);

	return seconds;
}

void TIMER_IF_BkUp_Write_Seconds(uint32_t Seconds) {
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SECONDS, Seconds);
}

void TIMER_IF_BkUp_Write_SubSeconds(uint32_t SubSeconds) {
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SUBSECONDS, SubSeconds);
}

uint32_t TIMER_IF_BkUp_Read_Seconds(void) {
	uint32_t ret = 0;
	ret = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SECONDS);

	return ret;
}

uint32_t TIMER_IF_BkUp_Read_SubSeconds(void) {
	uint32_t ret = 0;

	ret = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SUBSECONDS);

	return ret;
}



/* Private functions ---------------------------------------------------------*/
static void TIMER_IF_BkUp_Write_MSBticks(uint32_t MSBticks) {
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_MSBTICKS, MSBticks);
}

static uint32_t TIMER_IF_BkUp_Read_MSBticks(void) {
	uint32_t MSBticks;
	MSBticks = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_MSBTICKS);

	return MSBticks;
}

static inline uint64_t GetTimerTicks(void) {
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	return datetime_to_milisec(gDate.Year, gDate.Month, gDate.Date, gTime.Hours, gTime.Minutes, gTime.Seconds, gTime.SubSeconds);
}

