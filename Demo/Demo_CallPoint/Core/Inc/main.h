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
#include "stm32u0xx_hal.h"

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
#define BATT_Pin GPIO_PIN_1
#define BATT_GPIO_Port GPIOA
#define LORA_INT_Pin GPIO_PIN_3
#define LORA_INT_GPIO_Port GPIOA
#define LORA_INT_EXTI_IRQn EXTI2_3_IRQn
#define LORA_CS_Pin GPIO_PIN_4
#define LORA_CS_GPIO_Port GPIOA
#define MOTOR_INB_Pin GPIO_PIN_10
#define MOTOR_INB_GPIO_Port GPIOB
#define LORA_RST_Pin GPIO_PIN_8
#define LORA_RST_GPIO_Port GPIOA
#define LORA_BUSY_Pin GPIO_PIN_9
#define LORA_BUSY_GPIO_Port GPIOA
#define EN_12V_Pin GPIO_PIN_10
#define EN_12V_GPIO_Port GPIOA
#define BTN_WAKE_Pin GPIO_PIN_11
#define BTN_WAKE_GPIO_Port GPIOA
#define BTN_WAKE_EXTI_IRQn EXTI4_15_IRQn
#define MOTOR_INA_Pin GPIO_PIN_9
#define MOTOR_INA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
