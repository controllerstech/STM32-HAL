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
#include "stm32h7xx_hal.h"

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
#define COIL2_Pin GPIO_PIN_0
#define COIL2_GPIO_Port GPIOC
#define COIL5_Pin GPIO_PIN_2
#define COIL5_GPIO_Port GPIOC
#define COIL3_Pin GPIO_PIN_3
#define COIL3_GPIO_Port GPIOC
#define COIL1_Pin GPIO_PIN_3
#define COIL1_GPIO_Port GPIOA
#define COIL4_Pin GPIO_PIN_1
#define COIL4_GPIO_Port GPIOB
#define COIL6_Pin GPIO_PIN_11
#define COIL6_GPIO_Port GPIOF
#define COIL7_Pin GPIO_PIN_14
#define COIL7_GPIO_Port GPIOF
#define COIL8_Pin GPIO_PIN_15
#define COIL8_GPIO_Port GPIOF
#define COIL9_Pin GPIO_PIN_0
#define COIL9_GPIO_Port GPIOD
#define COIL10_Pin GPIO_PIN_1
#define COIL10_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
