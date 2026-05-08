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
extern uint8_t Event[];

uint8_t HadleButtons();
void SetRTC(const char* buf);
void GetRTC(void);

int RequestTimedateToC3(void);

__attribute__((noinline)) void delay_us(uint32_t us);

#endif /* INC_UTILS_H_ */
