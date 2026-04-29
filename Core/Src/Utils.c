/*
 * Utils.c
 *
 *  Created on: 21 apr 2026
 *      Author: molto
 */

#include "main.h"

volatile uint32_t ms_ticks = 0;

/* Funzione per leggere i tick (il tuo nuovo HAL_GetTick) */
uint32_t GetTick(void)
{
    return ms_ticks;
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
