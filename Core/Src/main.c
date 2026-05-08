/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Utils.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile char     rx_buffer[RX_BUFFER_SIZE] = {0};
volatile uint8_t  rx_index    = 0;
volatile uint8_t  rx_complete = 0;

volatile uint8_t  C3SyncReq = 0;

_NixieDisplay NixieDisplay;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//#define FIRST_STARTUP_DELAY		(61*1000+10)
#define FIRST_STARTUP_DELAY		(11*1000)
#define SEPARATOR_DELAYms		500
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 3);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LL_SYSTICK_EnableIT();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
  // set HIGT VOLTAGE to OFF!
  HV_OFF;
  HV_OFF;

  //tutto spento!
  ANODECATHODE_OFF;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t tickstart = GetTick();
  uint32_t separator = GetTick();

  /*

  TEST MODE!

  LL_GPIO_SetOutputPin(ANODE_SU_GPIO_Port, ANODE_SU_Pin);
  LL_GPIO_SetOutputPin(CATHODE_0_GPIO_Port, CATHODE_0_Pin);
  HV_ON;
  HV_OFF;
  */

  HV_ON;
  
  uint32_t startup = 0;
  int val = 0;

  while (1)
  {
	  if (GetTick() > tickstart )	//refresh value from RTC
	  {
		  GetRTC();
		  tickstart = GetTick() + 200;
	  }
	  if (GetTick() > separator )	//blink 'leds'
	  {
		  LL_GPIO_TogglePin(SEPARATOR_GPIO_Port, SEPARATOR_Pin);
		  separator = GetTick() + SEPARATOR_DELAYms;
	  }

	  if (startup == 0 && GetTick() > FIRST_STARTUP_DELAY )
		  C3SyncReq = 1;


	  HandleButtons();

	  if (Event[0])
	  {
		  LL_GPIO_SetOutputPin(C3_RESET_GPIO_Port, C3_RESET_Pin);
		  C3SyncReq = 1;
		  Event[0] = 0;
	  }

	  if (Event[1])
	  {
		  LL_GPIO_ResetOutputPin(C3_RESET_GPIO_Port, C3_RESET_Pin);
		  Event[1] = 0;
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    val = RequestTimedateToC3();
    if (val == 0)
    {
    	val = 0;
    	startup = 2;
    }
    else if (val == -1)
    {
    	//NOK: ricezione sbagliata!!!
    	val = 0;
    }
    else if (val == -2)
    {
    	//NOK: Timeout!!!
    	val = 0;
    }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
  }
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
  LL_RCC_EnableRTC();
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }

  LL_Init1msTick(16000000);

  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */

void LPUART1_Callback(uint8_t data)
{
	if (data == '@') {
		rx_index = 0; // Inizio di un nuovo messaggio
	}
	//else return;

	if (rx_index < RX_BUFFER_SIZE) {
		rx_buffer[rx_index++] = data;

		if (data == '\n' || data == '\r')
		{
			/* Fine stringa */
			rx_buffer[rx_index] = '\0';
			rx_complete = 1;
		}
			// Qui chiami la tua funzione parsetime(&t, &d, rx_buffer)
	}
}

//To be removed (?)
void User_Button_IT_Handler()
{
	C3SyncReq = 1;

//	LL_GPIO_SetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
}



void TIM2_ISR_Handle()
{
	static uint8_t current_tube = 0;
	// 1. Blanking (Spegni anodi e attendi)
	ANODECATHODE_OFF;
	//delay_us(0);

	SetNixie(current_tube);

	// 2. Prossimo tubo
	current_tube = (current_tube + 1) % 6;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
