#include "stm32f429xx.h"
#include "../Inc/switchMode.h"
#include "../Inc/master.h"


//stellt beide Pins auf PushPull - richtiger Port??
//nicht geshiftet mit << PIN, weil es pin 0 ist - passt das?
void PushPullAn()
{
    GPIOG->OTYPER &= ~(1 << 0);
}


//stellt pg0 auf OpenDrain - ist es richtiger Port????
void OpenDrainAn()
{
    GPIOG->OTYPER |= (1 << 0);
}
