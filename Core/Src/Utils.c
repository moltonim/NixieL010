/*
 * Utils.c
 *
 *  Created on: 21 apr 2026
 *      Author: molto
 */

#include <string.h>

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define C3_TIMEOUTms	5000


volatile uint32_t ms_ticks = 0;
LL_RTC_TimeTypeDef myTime = {0};

uint32_t day, hour, minute, second;


static int parsetime(LL_RTC_TimeTypeDef *t, const char* buf)
{
//    int n = 0;
    int p = strlen(buf);
    if (p != 8)
    	return -2;

    p = 0;
    if (buf[p++] != '@')
        return -1;
    //d.da_year = (n = (buf[p++]-'0')*1000+(buf[p++]-'0')*100+(buf[p++]-'0')*10+(buf[p++]-'0'));
    /*
    n += (buf[p++]-'0')*1000;
    n += (buf[p++]-'0')*100;
    //n += (buf[p++]-'0')*10;
    n += (buf[p++]-'0')*10;
    n += (buf[p++]-'0');
    //d->Year = n;
    n  = (buf[p++]-'0')*10;
    n += (buf[p++]-'0');
    //d->Month = n;
    n  = (buf[p++]-'0')*10;
    n += buf[p++]-'0';
    //d->Day = n;
    */
    t->Hours    = (buf[p++]-'0') * 10;
    t->Hours   += (buf[p++]-'0');
    t->Minutes  = (buf[p++]-'0') * 10;
    t->Minutes += (buf[p++]-'0');
    t->Seconds  = (buf[p++]-'0') * 10;
    t->Seconds += (buf[p++]-'0');

    return 0;
}


/* Funzione per leggere i tick (il tuo nuovo HAL_GetTick) */
uint32_t GetTick(void)
{
    return ms_ticks;
}



void SetRTC(const char* buf)
{
	LL_RTC_DateTypeDef d_dummy;

	// Valori fissi che l'hardware accetta senza problemi
	d_dummy.Day     = 1;
	d_dummy.Month   = LL_RTC_MONTH_JANUARY;
	d_dummy.Year    = 0; // Anno 2000
	d_dummy.WeekDay = LL_RTC_WEEKDAY_MONDAY;

	if (parsetime(&myTime, buf))
		return;

	// 1. Sblocca i registri RTC
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_EnterInitMode(RTC);

	// 2. Scrivi l'ora
	// Nota: LL_RTC_TIME_FORMAT_AM_OR_24 � necessario
	LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24, myTime.Hours, myTime.Minutes, myTime.Seconds);

	// 3. Scrivi la data
	//LL_RTC_DATE_Config(RTC, LL_RTC_WEEKDAY_FRIDAY, myDate.Day, myDate.Month, myDate.Year);
	LL_RTC_DATE_Config(RTC, d_dummy.WeekDay, d_dummy.Day, d_dummy.Month, d_dummy.Year);

	// 4. Chiudi e riproteggi
	LL_RTC_ExitInitMode(RTC);
	LL_RTC_EnableWriteProtection(RTC);
}


void GetRTC(void)
{

	// 1. Leggi il registro del tempo (contiene HH:MM:SS in formato BCD)
	uint32_t temp_time = LL_RTC_TIME_Get(RTC);

	// 2. IMPORTANTE: Leggi sempre la DATA dopo il tempo per sbloccare i registri ombra
	(void)LL_RTC_DATE_Get(RTC);

	// 3. Estrai i valori usando le macro LL e converti in decimale (Binario)
	//day = __LL_RTC_GET_DAY(temp_time);

	hour   = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_HOUR(temp_time));
	minute = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_MINUTE(temp_time));
	second = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_SECOND(temp_time));

	DISABLE_TIM2;
	display_buffer[0] = hour/10;
	display_buffer[1] = hour%10;

	display_buffer[2] = minute/10;
	display_buffer[3] = minute%10;

	display_buffer[4] = second/10;
	display_buffer[5] = second%10;
	ENABLE_TIM2;
}



int RequestTimedateToC3(void)
{
	if (hour == 2 && !minute && !second)
	{
		//sono le 2 di notte: cerco data/ora
		if (day != 2)
			return 1;
	}

	//disable TIM2
	DISABLE_TIM2;
	//switch HiV OFF
	HV_OFF;
	//wait a little
	LL_mDelay(50);
	//Ask C3 to getup
	C3_GETUP;
	int ms = GetTick() + C3_TIMEOUTms;
	char buf[RX_BUFFER_SIZE] = {};

	rx_complete = 0;
	rx_index = 0;

	while (ms > GetTick() )
	{

		if (rx_complete)
		{
			__disable_irq(); // Disabilita tutti gli interrupt
			//rx_complete = 0;
			memcpy(buf, (const char*)&rx_buffer[0], rx_index);
			rx_index = 0;
			__enable_irq();  // Riabilita gli interrupt

			//if (!strncmp("T20", buf, 3))
			if (buf[0] == '@')
			{
				//if (parsetime(&myTime, buf) == 0)
				{
					SetRTC(buf);
				}
				rx_complete = 2;
				//LL_GPIO_TogglePin(LEDY_GPIO_Port, LEDY_Pin);
				LL_GPIO_SetOutputPin(LED_G_GPIO_Port, LED_G_Pin);

				LL_GPIO_ResetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
				LL_GPIO_ResetOutputPin(GETUP_GPIO_Port, GETUP_Pin);

				break;
			}
		}
	}	//end while

	//C3 goto sleep
	C3_GETDOWN;

	//turn /HV on
	HV_ON;

	if (rx_complete == 1)
	{
		//NOK: ricezione sbagliata!!!
		return -1;
	}

	if (rx_complete == 0)
	{
		//NOK: Timeout!!!!
		return -2;
	}

	if (rx_complete == 2)
	{
		//OK: sincronismo avvenuto.
		return 0;
	}
	return -10;
}



__attribute__((noinline)) // Impedisce al compilatore di integrare la funzione (cambierebbe i tempi)
void delay_us(uint32_t us)
{
    // 1. Calcola quanti cicli di "loop" servono.
    // SystemCoreClock / 1.000.000 ci dà i cicli per 1 microsecondo.
    // Dividiamo per un fattore di calibrazione (tipicamente tra 4 e 8 per M0+)
    // Per STM32L0 a 32MHz, il fattore circa 8 è molto comune.

    uint32_t count = us * (SystemCoreClock / 8000000U);

    if (count == 0)
    	count = 1;

    /*
    // 2. Loop in assembly per garantire determinismo
    __asm volatile (
    		"1: subs %0, #1 \n\t" // 1 ciclo
    		"bne 1b         \n\t" // 1 ciclo se falso, 3 cicli se vero (salto)
    		: "+r" (count)
    );*/

    __asm volatile (
    		".syntax unified    \n\t" // Forza la sintassi moderna
    		"1: subs %0, #1 \n\t"
    		"bne 1b         \n\t"
    		: "+l" (count) // Forza l'uso di un registro tra r0 e r7
			:
			: "cc"         // Indica che l'operazione modifica i flag di condizione
    );

}
