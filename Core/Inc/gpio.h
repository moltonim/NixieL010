/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define HV_OFF	LL_GPIO_ResetOutputPin(HV_ENABLE_GPIO_Port, HV_ENABLE_Pin)
#define HV_ON	LL_GPIO_SetOutputPin(HV_ENABLE_GPIO_Port, HV_ENABLE_Pin)

#define VAC_ON	LL_GPIO_IsInputPinSet(Vac_OFF_GPIO_Port, Vac_OFF_Pin)

#define C3_GETUP	LL_GPIO_SetOutputPin(GETUP_GPIO_Port, GETUP_Pin)
#define C3_GETDOWN	LL_GPIO_ResetOutputPin(GETUP_GPIO_Port, GETUP_Pin)

#define	ANODECATHODE_OFF		WRITE_REG(CATHODE_0_GPIO_Port->BRR, 0xFFFF)


/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

void SetNixie(uint8_t tube);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

