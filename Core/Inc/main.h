/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define PWR_5V_EN_GPIO_Pin GPIO_PIN_5
#define PWR_5V_EN_GPIO_GPIO_Port GPIOA
#define ETH_RST_GPIO_Pin GPIO_PIN_7
#define ETH_RST_GPIO_GPIO_Port GPIOE
#define ETH_INT_GPIO_Pin GPIO_PIN_8
#define ETH_INT_GPIO_GPIO_Port GPIOE
#define ETH_RXERR_GPIO_Pin GPIO_PIN_9
#define ETH_RXERR_GPIO_GPIO_Port GPIOE
#define SPI2_NSS2_GPIO_Pin GPIO_PIN_14
#define SPI2_NSS2_GPIO_GPIO_Port GPIOE
#define SPI2_NSS1_GPIO_Pin GPIO_PIN_15
#define SPI2_NSS1_GPIO_GPIO_Port GPIOE
#define EEPROM_WRITE_GPIO_Pin GPIO_PIN_11
#define EEPROM_WRITE_GPIO_GPIO_Port GPIOD
#define EEPROM_HOLD_GPIO_Pin GPIO_PIN_12
#define EEPROM_HOLD_GPIO_GPIO_Port GPIOD
#define MCP_RESET_GPIO_Pin GPIO_PIN_13
#define MCP_RESET_GPIO_GPIO_Port GPIOD
#define MCP_INTA_GPIO_Pin GPIO_PIN_14
#define MCP_INTA_GPIO_GPIO_Port GPIOD
#define MCP_INTB_GPIO_Pin GPIO_PIN_15
#define MCP_INTB_GPIO_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
