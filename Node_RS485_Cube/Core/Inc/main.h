/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define RF_CTRL1_Pin GPIO_PIN_8
#define RF_CTRL1_GPIO_Port GPIOB
#define LED_ACT_Pin GPIO_PIN_4
#define LED_ACT_GPIO_Port GPIOA
#define MB_DE_Pin GPIO_PIN_5
#define MB_DE_GPIO_Port GPIOA
#define MB_RE_Pin GPIO_PIN_6
#define MB_RE_GPIO_Port GPIOA
#define SENSOR_PWR_Pin GPIO_PIN_7
#define SENSOR_PWR_GPIO_Port GPIOA
#define CHMOD_BTN_Pin GPIO_PIN_9
#define CHMOD_BTN_GPIO_Port GPIOA
#define CHMOD_BTN_EXTI_IRQn EXTI9_5_IRQn
#define VBAT_Pin GPIO_PIN_2
#define VBAT_GPIO_Port GPIOB
#define MB_PWR_Pin GPIO_PIN_12
#define MB_PWR_GPIO_Port GPIOA
#define RF_CTRL2_Pin GPIO_PIN_13
#define RF_CTRL2_GPIO_Port GPIOC
#define WF_PWR_Pin GPIO_PIN_15
#define WF_PWR_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
