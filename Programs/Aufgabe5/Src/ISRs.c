#include "lcd.h"
#include "stm32f429xx.h"
#include <stdbool.h>

extern volatile bool s6;
extern volatile int counter;
extern volatile int counter1;
extern volatile int counter0;

//muss man so nennen, damit CPU interruptfunktion findet
void EXTI9_5_IRQHandler(void)
{
    EXTI->PR = (1<<6);
    s6 = true;
    counter++;
}

void EXTI0_IRQHandler(void)
{
    EXTI->PR = (1<<0);
    counter0++;
}

void EXTI1_IRQHandler(void)
{
    EXTI->PR = (1<<1);
    counter1++;
}




