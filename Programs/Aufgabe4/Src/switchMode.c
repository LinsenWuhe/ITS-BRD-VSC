#include "stm32f429xx.h"
#include "../Inc/switchMode.h"

void aufPushPullStellen()
{
    GPIOG->OTYPER &= ~(1<< PD0);
    GPIOG->OTYPER &= ~(1<< PD1);
}


//stellt beide Pins auf OpenDrain - ist es richtiger Port????
void aufOpenDrainStellen()
{
    GPIOG->OTYPER |= (1<<PD0);
    GPIOG->OTYPER |= (1<<PD1);
}
