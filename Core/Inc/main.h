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
#include "stm32l0xx_hal.h"

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
#define RX_BUFFER_SIZE 128

extern UART_HandleTypeDef hlpuart1;
extern volatile uint8_t  rx_byte;
extern volatile char     rx_buffer[RX_BUFFER_SIZE];
extern volatile uint8_t  rx_index;
extern volatile uint8_t  rx_complete;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HV_ENABLE_Pin GPIO_PIN_0
#define HV_ENABLE_GPIO_Port GPIOC
#define SEPARATOR_Pin GPIO_PIN_13
#define SEPARATOR_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_5
#define LED_G_GPIO_Port GPIOA
#define LED_Y_Pin GPIO_PIN_6
#define LED_Y_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_7
#define LED_R_GPIO_Port GPIOA
#define ANODE_HD_Pin GPIO_PIN_0
#define ANODE_HD_GPIO_Port GPIOB
#define ANODE_HU_Pin GPIO_PIN_1
#define ANODE_HU_GPIO_Port GPIOB
#define ANODE_MD_Pin GPIO_PIN_2
#define ANODE_MD_GPIO_Port GPIOB
#define CATHODE_4_Pin GPIO_PIN_10
#define CATHODE_4_GPIO_Port GPIOB
#define CATHODE_5_Pin GPIO_PIN_11
#define CATHODE_5_GPIO_Port GPIOB
#define CATHODE_6_Pin GPIO_PIN_12
#define CATHODE_6_GPIO_Port GPIOB
#define CATHODE_7_Pin GPIO_PIN_13
#define CATHODE_7_GPIO_Port GPIOB
#define CATHODE_8_Pin GPIO_PIN_14
#define CATHODE_8_GPIO_Port GPIOB
#define CATHODE_9_Pin GPIO_PIN_15
#define CATHODE_9_GPIO_Port GPIOB
#define Vac_OFF_Pin GPIO_PIN_15
#define Vac_OFF_GPIO_Port GPIOA
#define ANODE_MU_Pin GPIO_PIN_3
#define ANODE_MU_GPIO_Port GPIOB
#define ANODE_SD_Pin GPIO_PIN_4
#define ANODE_SD_GPIO_Port GPIOB
#define ANODE_SU_Pin GPIO_PIN_5
#define ANODE_SU_GPIO_Port GPIOB
#define CATHODE_0_Pin GPIO_PIN_6
#define CATHODE_0_GPIO_Port GPIOB
#define CATHODE_1_Pin GPIO_PIN_7
#define CATHODE_1_GPIO_Port GPIOB
#define CATHODE_2_Pin GPIO_PIN_8
#define CATHODE_2_GPIO_Port GPIOB
#define CATHODE_3_Pin GPIO_PIN_9
#define CATHODE_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
