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
#include "Utils.h"

#define C3_TIMEOUTms	15000
#define COUNTDOWNs		11

volatile uint32_t ms_ticks = 0;

uint8_t Event[NUMEVENT];
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



int  SetRTC(const char* buf)
{
	LL_RTC_DateTypeDef d_dummy;

	// Valori fissi che l'hardware accetta senza problemi
	d_dummy.Day     = 1;
	d_dummy.Month   = LL_RTC_MONTH_JANUARY;
	d_dummy.Year    = 0; // Anno 2000
	d_dummy.WeekDay = LL_RTC_WEEKDAY_MONDAY;

	if (parsetime(&myTime, buf))
		return -1;

	// 1. Sblocca i registri RTC
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_EnterInitMode(RTC);

	// 2. Scrivi l'ora
	// Nota: LL_RTC_TIME_FORMAT_AM_OR_24 � necessario
	//LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24, myTime.Hours, myTime.Minutes, myTime.Seconds);
	LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24,
			__LL_RTC_CONVERT_BIN2BCD(myTime.Hours),
			__LL_RTC_CONVERT_BIN2BCD(myTime.Minutes),
			__LL_RTC_CONVERT_BIN2BCD(myTime.Seconds)
	);

	// 3. Scrivi la data
	//LL_RTC_DATE_Config(RTC, LL_RTC_WEEKDAY_FRIDAY, myDate.Day, myDate.Month, myDate.Year);
	LL_RTC_DATE_Config(RTC, d_dummy.WeekDay, d_dummy.Day, d_dummy.Month, d_dummy.Year);

	// 4. Chiudi e riproteggi
	LL_RTC_ExitInitMode(RTC);
	LL_RTC_EnableWriteProtection(RTC);
	return 0;
}


uint8_t HandleButtons( uint8_t reset)
{
	static uint32_t ButtonPin[3] = {
			BTTN1_Pin,
			BTTN2_Pin,
			BTTN3_Pin
	};
	static uint32_t bttn[3];
	uint32_t n;

	if (reset)
	{
		memset(bttn, 0, sizeof(bttn));
		memset(Event, 0, sizeof(Event));
		return 0;
	}

	for (int i = 0; i <(sizeof(ButtonPin)/sizeof(ButtonPin[0])); i++)
	{
//		n = LL_GPIO_IsInputPinSet(BTTN1_GPIO_Port, ButtonPin[i]);
		n = LL_GPIO_IsInputPinSet(BTTN1_GPIO_Port, ButtonPin[i]);
		if (!n && !bttn[i])
			bttn[i] = GetTick() + 1500;
		else if (n)
			bttn[i] = 0;

		if (bttn[i] && GetTick() > bttn[i] && !Event[1])
		{
			Event[i] = 1;
		}
	}
	return Event[0] | (Event[1]<<1) | (Event[2]<<2);
}


//countdown in secondi: mostra un conuntdown sino a spegnimento!
void CountDown(void)
{
	static uint32_t dwn;

	if (!dwn)
	{
		dwn = GetTick();
//		dwn /= 1000;		//to seconds
//		dwn %= (24*3600);	//remove day(s)
		dwn += NixieDisplay.cdown ;		//starting from 12 sec (11?)
		return;
	}
	uint32_t n = dwn - GetTick();
	uint8_t ev = 0;
	if (n > 0x00FFFFFF)		//< 0
	{
		Event[3] = 1;
		return;
	}

	ev = HandleButtons(0);
	if (ev != 5)
	{
		NixieDisplay.DisplayMode = DISPLAYMODE_NORMAL;
		dwn = 0;
		return;
	}

	n /= 1000;		//to seconds
	n %= (24*3600);	//remove day(s)


	DISABLE_TIM2;

	NixieDisplay.display_buffer[3] = n%10;
	if (!n)
		n = 1;
	NixieDisplay.display_buffer[2] = n/10;

	ENABLE_TIM2;
}




void GetRTC(void)
{

	// 1. Leggi il registro del tempo (contiene HH:MM:SS in formato BCD)
	uint32_t temp_time = LL_RTC_TIME_Get(RTC);

	// 2. IMPORTANTE: Leggi sempre la DATA dopo il tempo per sbloccare i registri ombra
	uint32_t temp_date = LL_RTC_DATE_Get(RTC);

	// 3. Estrai i valori usando le macro LL e converti in decimale (Binario)
	day = __LL_RTC_GET_DAY(temp_date);

	hour   = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_HOUR(temp_time));
	minute = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_MINUTE(temp_time));
	second = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_SECOND(temp_time));

	DISABLE_TIM2;
	NixieDisplay.display_buffer[0] = hour/10;
	NixieDisplay.display_buffer[1] = hour%10;

	NixieDisplay.display_buffer[2] = minute/10;
	NixieDisplay.display_buffer[3] = minute%10;

	NixieDisplay.display_buffer[4] = second/10;
	NixieDisplay.display_buffer[5] = second%10;
	if (NixieDisplay.EvenSecs >= 5)
		NixieDisplay.display_buffer[5] &= 0xFE;

	ENABLE_TIM2;
}



int EraseSetupC3(void)
{
	//disable TIM2
	DISABLE_TIM2;
	//switch HiV OFF
	HV_OFF;
	//wait a little
	LL_mDelay(500);
	//Set ERASE on C3:
	C3_RESET_REQ;
	//Ask C3 to getup
	C3_GETUP;

	//wait a little
	LL_mDelay(500);
	//Ebd ERASE on C3:
	C3_RESET_END;

	//Ensure clock in normal mode:
	NixieDisplay.DisplayMode = DISPLAYMODE_NORMAL;

	//
	//	NOTA: Lascio lorario precedente!
	//

	ENABLE_TIM2;

	return 0;
}



int RequestTimedateToC3(void)
{

	if (!C3SyncReq)
	{
		if ((hour == 2 && !minute && !second))
		{
			//sono le 2 di notte: cerco data/ora
			if (day != 3)
				return 0;
			else
				C3SyncReq = 1;

		}
		else return 1;
	}

	//disable TIM2
	DISABLE_TIM2;
	//switch HiV OFF
	HV_OFF;
	//wait a little
	LL_mDelay(500);
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
					if (SetRTC(buf))
						return -1;
				}
				rx_complete = 2;

				break;
			}
		}
	}	//end while

	//C3 goto sleep
	C3_GETDOWN;
	C3SyncReq = 0;
	C3_RESET_END;	//force reset of C£'s request
	LL_mDelay(300);
	ENABLE_TIM2;
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
