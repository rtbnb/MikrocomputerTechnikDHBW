/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define B1_USER_BUTTON_Pin GPIO_PIN_13
#define B1_USER_BUTTON_GPIO_Port GPIOC
#define EDU_SEG_G_Pin GPIO_PIN_1
#define EDU_SEG_G_GPIO_Port GPIOC
#define EDU_POT1_Pin GPIO_PIN_0
#define EDU_POT1_GPIO_Port GPIOA
#define LD2_OUTPUT_Pin GPIO_PIN_5
#define LD2_OUTPUT_GPIO_Port GPIOA
#define EDU_LD1_Pin GPIO_PIN_10
#define EDU_LD1_GPIO_Port GPIOB
#define EDU_LD2_Pin GPIO_PIN_7
#define EDU_LD2_GPIO_Port GPIOC
#define SW1_EXT10_Pin GPIO_PIN_10
#define SW1_EXT10_GPIO_Port GPIOA
#define SW1_EXT10_EXTI_IRQn EXTI15_10_IRQn
#define EDU_SEG_F_Pin GPIO_PIN_15
#define EDU_SEG_F_GPIO_Port GPIOA
#define EDU_BUZ_Pin GPIO_PIN_4
#define EDU_BUZ_GPIO_Port GPIOB
#define EDU_LD0_Pin GPIO_PIN_5
#define EDU_LD0_GPIO_Port GPIOB
#define EDU_LD4_Pin GPIO_PIN_8
#define EDU_LD4_GPIO_Port GPIOB
#define EDU_LD3_Pin GPIO_PIN_9
#define EDU_LD3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
