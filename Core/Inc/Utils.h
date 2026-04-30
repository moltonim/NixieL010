/*
 * Utils.h
 *
 *  Created on: 21 apr 2026
 *      Author: molto
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

/* Funzione per leggere i tick (il tuo nuovo HAL_GetTick) */
uint32_t GetTick(void);
__attribute__((noinline)) void delay_us(uint32_t us);

#endif /* INC_UTILS_H_ */
