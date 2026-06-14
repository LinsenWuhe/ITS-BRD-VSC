#include "stm32f429xx.h"
#include "../Inc/switchMode.h"
#include "../Inc/master.h"


//stellt beide Pins auf PushPull - richtiger Port??
//nicht geshiftet mit << PIN, weil es pin 0 ist - passt das?
void aufPushPullStellen()
{
    GPIOG->OTYPER &= ~(1 << PIN);
}


//stellt beide Pins auf OpenDrain - ist es richtiger Port????
void aufOpenDrainStellen()
{
    GPIOG->OTYPER |= (1 << PIN);
}
