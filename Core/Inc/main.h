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
#define PC14_Pin GPIO_PIN_14
#define PC14_GPIO_Port GPIOC
#define PC15_Pin GPIO_PIN_15
#define PC15_GPIO_Port GPIOC
#define HEATCOIL_ELEMENT_Pin GPIO_PIN_0
#define HEATCOIL_ELEMENT_GPIO_Port GPIOC
#define EN_HEAT_Pin GPIO_PIN_1
#define EN_HEAT_GPIO_Port GPIOC
#define FAN_Pin GPIO_PIN_2
#define FAN_GPIO_Port GPIOC
#define PUMP_Pin GPIO_PIN_3
#define PUMP_GPIO_Port GPIOC
#define INT_RTC_Pin GPIO_PIN_0
#define INT_RTC_GPIO_Port GPIOA
#define Sound_Pin GPIO_PIN_1
#define Sound_GPIO_Port GPIOA
#define EN_Sound_Pin GPIO_PIN_2
#define EN_Sound_GPIO_Port GPIOA
#define LORA_RST_Pin GPIO_PIN_3
#define LORA_RST_GPIO_Port GPIOA
#define LORA_CS_Pin GPIO_PIN_4
#define LORA_CS_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA
#define LORA_INT_Pin GPIO_PIN_4
#define LORA_INT_GPIO_Port GPIOC
#define LORA_INT_EXTI_IRQn EXTI4_IRQn
#define LORA_BUSY_Pin GPIO_PIN_5
#define LORA_BUSY_GPIO_Port GPIOC
#define ADC_BAT_Pin GPIO_PIN_0
#define ADC_BAT_GPIO_Port GPIOB
#define MODE_Pin GPIO_PIN_1
#define MODE_GPIO_Port GPIOB
#define INA_Pin GPIO_PIN_10
#define INA_GPIO_Port GPIOB
#define INB_Pin GPIO_PIN_11
#define INB_GPIO_Port GPIOB
#define CS_NFC_Pin GPIO_PIN_12
#define CS_NFC_GPIO_Port GPIOB
#define SPI2_SCK_Pin GPIO_PIN_13
#define SPI2_SCK_GPIO_Port GPIOB
#define SPI2_MISO_Pin GPIO_PIN_14
#define SPI2_MISO_GPIO_Port GPIOB
#define SPI2_MOSI_Pin GPIO_PIN_15
#define SPI2_MOSI_GPIO_Port GPIOB
#define INT_NFC_Pin GPIO_PIN_6
#define INT_NFC_GPIO_Port GPIOC
#define EN_12V_Pin GPIO_PIN_7
#define EN_12V_GPIO_Port GPIOC
#define NFC_WAKE_Pin GPIO_PIN_8
#define NFC_WAKE_GPIO_Port GPIOC
#define NFC_WAKE_EXTI_IRQn EXTI9_5_IRQn
#define RST_RTC_Pin GPIO_PIN_8
#define RST_RTC_GPIO_Port GPIOA
#define I2C_SCL_Pin GPIO_PIN_9
#define I2C_SCL_GPIO_Port GPIOA
#define I2C_SDA_Pin GPIO_PIN_10
#define I2C_SDA_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
