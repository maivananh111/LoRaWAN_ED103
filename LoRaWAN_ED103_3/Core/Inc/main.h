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
#include "stm32l4xx_hal.h"

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
#define LORA_SPI hspi1
#define LOG_UART huart1
#define BATT_ADC hadc1
#define NFC_SPI hspi2
#define ADC_BATT_Pin GPIO_PIN_1
#define ADC_BATT_GPIO_Port GPIOA
#define LORA_INT_Pin GPIO_PIN_3
#define LORA_INT_GPIO_Port GPIOA
#define LORA_INT_EXTI_IRQn EXTI3_IRQn
#define LORA_CS_Pin GPIO_PIN_4
#define LORA_CS_GPIO_Port GPIOA
#define INB_Pin GPIO_PIN_10
#define INB_GPIO_Port GPIOB
#define LORA_RST_Pin GPIO_PIN_8
#define LORA_RST_GPIO_Port GPIOA
#define LORA_BUSY_Pin GPIO_PIN_9
#define LORA_BUSY_GPIO_Port GPIOA
#define P12V_EN_Pin GPIO_PIN_10
#define P12V_EN_GPIO_Port GPIOA
#define BTN_WAKE_Pin GPIO_PIN_11
#define BTN_WAKE_GPIO_Port GPIOA
#define BTN_WAKE_EXTI_IRQn EXTI15_10_IRQn
#define INA_Pin GPIO_PIN_9
#define INA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
