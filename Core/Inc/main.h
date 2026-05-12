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

#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
	DISPLAYMODE_NORMAL = 0,
	DISPLAYMODE_CDOWN,
	DISPLAYMODE_BLINK_S
}_DisplayMode;
typedef struct {
	uint8_t display_buffer[6];
	//uint8_t BlinkMode;
	uint32_t EvenSecs;			//NON mostro i secondi pari dopo 5 errori
	_DisplayMode DisplayMode;
	uint32_t cdown;
} _NixieDisplay;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define RX_BUFFER_SIZE 50

extern _NixieDisplay NixieDisplay;

extern volatile uint8_t  rx_byte;
extern volatile char     rx_buffer[RX_BUFFER_SIZE];
extern volatile uint8_t  rx_index;
extern volatile uint8_t  rx_complete;
extern volatile uint8_t  C3SyncReq;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void User_Button_IT_Handler(void);
void LPUART1_Callback(uint8_t rx_byte);
void TIM2_ISR_Handle(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HV_ENABLE_Pin LL_GPIO_PIN_0
#define HV_ENABLE_GPIO_Port GPIOC
#define SEPARATOR_Pin LL_GPIO_PIN_13
#define SEPARATOR_GPIO_Port GPIOC
#define GETUP_Pin LL_GPIO_PIN_0
#define GETUP_GPIO_Port GPIOA
#define C3_RESET_Pin LL_GPIO_PIN_1
#define C3_RESET_GPIO_Port GPIOA
#define BTTN1_Pin LL_GPIO_PIN_5
#define BTTN1_GPIO_Port GPIOA
#define BTTN2_Pin LL_GPIO_PIN_6
#define BTTN2_GPIO_Port GPIOA
#define BTTN3_Pin LL_GPIO_PIN_7
#define BTTN3_GPIO_Port GPIOA
#define ANODE_HD_Pin LL_GPIO_PIN_0
#define ANODE_HD_GPIO_Port GPIOB
#define ANODE_HU_Pin LL_GPIO_PIN_1
#define ANODE_HU_GPIO_Port GPIOB
#define ANODE_MD_Pin LL_GPIO_PIN_2
#define ANODE_MD_GPIO_Port GPIOB
#define CATHODE_4_Pin LL_GPIO_PIN_10
#define CATHODE_4_GPIO_Port GPIOB
#define CATHODE_5_Pin LL_GPIO_PIN_11
#define CATHODE_5_GPIO_Port GPIOB
#define CATHODE_6_Pin LL_GPIO_PIN_12
#define CATHODE_6_GPIO_Port GPIOB
#define CATHODE_7_Pin LL_GPIO_PIN_13
#define CATHODE_7_GPIO_Port GPIOB
#define CATHODE_8_Pin LL_GPIO_PIN_14
#define CATHODE_8_GPIO_Port GPIOB
#define CATHODE_9_Pin LL_GPIO_PIN_15
#define CATHODE_9_GPIO_Port GPIOB
#define Vac_OFF_Pin LL_GPIO_PIN_15
#define Vac_OFF_GPIO_Port GPIOA
#define ANODE_MU_Pin LL_GPIO_PIN_3
#define ANODE_MU_GPIO_Port GPIOB
#define ANODE_SD_Pin LL_GPIO_PIN_4
#define ANODE_SD_GPIO_Port GPIOB
#define ANODE_SU_Pin LL_GPIO_PIN_5
#define ANODE_SU_GPIO_Port GPIOB
#define CATHODE_0_Pin LL_GPIO_PIN_6
#define CATHODE_0_GPIO_Port GPIOB
#define CATHODE_1_Pin LL_GPIO_PIN_7
#define CATHODE_1_GPIO_Port GPIOB
#define CATHODE_2_Pin LL_GPIO_PIN_8
#define CATHODE_2_GPIO_Port GPIOB
#define CATHODE_3_Pin LL_GPIO_PIN_9
#define CATHODE_3_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
