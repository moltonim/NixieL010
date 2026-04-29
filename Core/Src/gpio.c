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
}


void SetSeparator(uint8_t val)
{
	HAL_GPIO_WritePin(SEPARATOR_GPIO_Port, SEPARATOR_Pin, val);
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

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, HV_ENABLE_Pin|SEPARATOR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_G_Pin|LED_Y_Pin|LED_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ANODE_HD_Pin|ANODE_HU_Pin|ANODE_MD_Pin|CATHODE_4_Pin
                          |CATHODE_5_Pin|CATHODE_6_Pin|CATHODE_7_Pin|CATHODE_8_Pin
                          |CATHODE_9_Pin|ANODE_MU_Pin|ANODE_SD_Pin|ANODE_SU_Pin
                          |CATHODE_0_Pin|CATHODE_1_Pin|CATHODE_2_Pin|CATHODE_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : HV_ENABLE_Pin SEPARATOR_Pin */
  GPIO_InitStruct.Pin = HV_ENABLE_Pin|SEPARATOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PH0 PH1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 PA8
                           PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_G_Pin LED_Y_Pin LED_R_Pin */
  GPIO_InitStruct.Pin = LED_G_Pin|LED_Y_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ANODE_HD_Pin ANODE_HU_Pin ANODE_MD_Pin CATHODE_4_Pin
                           CATHODE_5_Pin CATHODE_6_Pin CATHODE_7_Pin CATHODE_8_Pin
                           CATHODE_9_Pin ANODE_MU_Pin ANODE_SD_Pin ANODE_SU_Pin
                           CATHODE_0_Pin CATHODE_1_Pin CATHODE_2_Pin CATHODE_3_Pin */
  GPIO_InitStruct.Pin = ANODE_HD_Pin|ANODE_HU_Pin|ANODE_MD_Pin|CATHODE_4_Pin
                          |CATHODE_5_Pin|CATHODE_6_Pin|CATHODE_7_Pin|CATHODE_8_Pin
                          |CATHODE_9_Pin|ANODE_MU_Pin|ANODE_SD_Pin|ANODE_SU_Pin
                          |CATHODE_0_Pin|CATHODE_1_Pin|CATHODE_2_Pin|CATHODE_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Vac_OFF_Pin */
  GPIO_InitStruct.Pin = Vac_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Vac_OFF_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
