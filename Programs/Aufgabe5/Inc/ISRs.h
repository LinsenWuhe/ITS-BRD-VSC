#ifndef __ISRS_H
#define __ISRS_H

#include <stdint.h>

void EXTI9_5_IRQHandler(void);

void EXTI0_IRQHandler(void);

void EXTI1_IRQHandler(void);

void get_drehgeberdaten(uint32_t *zeitstempel_out, uint32_t *zaehler_out);

#endif