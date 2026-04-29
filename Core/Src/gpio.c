/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
/*
 * NOTA: dipende tutto da come funzionano i nixie:
 * prima accendo il catodo (che rimane spento)
 * Poi accendo l'anodo - che accende quello che mi serve)
 *
 * il numero successiov:
 * Spengo l'anodo, accendo il catodo, accendop l'altro anodo!
 */


/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#define	ANODECATHODE_OFF		CATHODE_0_GPIO_Port->BRR  = 0x0000FFFF;
#define	CATHODE_OFF				CATHODE_0_GPIO_Port->BRR  = 0x0000FFC0;

#define RITARDO_1				165		// ~1000/6

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
void Set_Cathode(uint8_t val)
{
	uint32_t port;
	uint32_t t = 1;

	val %= 10;			//Avoid extravalue
	//if (val == 10) val = 0;

	port =0x0000FFC0;	//remove most sign. unused variable
	t >>= 6;			//jump anode settings
	t >>= val;			//

	//GPIOx->BRR  = port;	//reset ALL value
	port = port;
	// NOTA: da sostituire con 'CATHODE_OFF' ed inserire un ritardo!

	/* VALUTARE RITARDO TRA SPEGNIMENTO ED ACCENSIONE. DIPENDE DA NIXIE!  */
	CATHODE_0_GPIO_Port->BSRR = t;	//set *only* interesting value
}

void Set_Anode(uint8_t val)
{
	uint32_t port = ANODE_HD_GPIO_Port->BSRR;
	uint32_t t = 1;

	val %= 6;			//from 0 to 5
	t >>= val;
	t |= port;

	ANODE_HD_GPIO_Port->BSRR = t;	//set *only* interesting value
}


void Set_NixieTime(uint8_t h, uint8_t m, uint8_t s)
{
#if 0
	Set_Cathode(h/10);
	Set_Anode(0);
	HAL_Delay(RITARDO_1);
	ANODECATHODE_OFF;

	Set_Cathode(h%10);
	Set_Anode(1);
	HAL_Delay(RITARDO_1);
	ANODECATHODE_OFF;

	Set_Cathode(m/10);
	Set_Anode(2);
	HAL_Delay(RITARDO_1);
	ANODECATHODE_OFF;

	Set_Cathode(m%10);
	Set_Anode(3);
	HAL_Delay(RITARDO_1);
	ANODECATHODE_OFF;

	Set_Cathode(s/10);
	Set_Anode(4);
	HAL_Delay(RITARDO_1);
	ANODECATHODE_OFF;

	Set_Cathode(s%10);
	Set_Anode(5);
	HAL_Delay(RITARDO_1);

	// L?ultimo OFF non serve: devo solamente aspettare la nuova ora?
	//ANODECATHODE_OFF;
#endif
}


void SetSeparator(uint8_t val)
{
	//HAL_GPIO_WritePin(SEPARATOR_GPIO_Port, SEPARATOR_Pin, val);
}



/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOH);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(HV_ENABLE_GPIO_Port, HV_ENABLE_Pin);

  /**/
  LL_GPIO_ResetOutputPin(SEPARATOR_GPIO_Port, SEPARATOR_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GETUP_GPIO_Port, GETUP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED_G_GPIO_Port, LED_G_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED_Y_GPIO_Port, LED_Y_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED_R_GPIO_Port, LED_R_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_HD_GPIO_Port, ANODE_HD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_HU_GPIO_Port, ANODE_HU_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_MD_GPIO_Port, ANODE_MD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_4_GPIO_Port, CATHODE_4_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_5_GPIO_Port, CATHODE_5_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_6_GPIO_Port, CATHODE_6_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_7_GPIO_Port, CATHODE_7_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_8_GPIO_Port, CATHODE_8_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_9_GPIO_Port, CATHODE_9_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_MU_GPIO_Port, ANODE_MU_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_SD_GPIO_Port, ANODE_SD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ANODE_SU_GPIO_Port, ANODE_SU_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_0_GPIO_Port, CATHODE_0_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_1_GPIO_Port, CATHODE_1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_2_GPIO_Port, CATHODE_2_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CATHODE_3_GPIO_Port, CATHODE_3_Pin);

  /**/
  GPIO_InitStruct.Pin = HV_ENABLE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(HV_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SEPARATOR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SEPARATOR_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = GETUP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GETUP_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_G_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_G_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_Y_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_Y_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_R_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_R_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_HD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_HD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_HU_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_HU_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_MD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_MD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_4_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_5_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_5_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_6_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_6_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_7_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_7_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_8_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_8_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_9_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_9_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Vac_OFF_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(Vac_OFF_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_MU_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_MU_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_SD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_SD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ANODE_SU_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ANODE_SU_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_0_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CATHODE_3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CATHODE_3_GPIO_Port, &GPIO_InitStruct);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);

  /**/
  LL_GPIO_SetPinPull(BUTTON_GPIO_Port, BUTTON_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(BUTTON_GPIO_Port, BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI0_1_IRQn, 0);
  NVIC_EnableIRQ(EXTI0_1_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
