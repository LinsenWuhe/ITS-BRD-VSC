#include "io.h"
#include "stm32f429xx.h"

bool buttonPressed(int n)
{
    //Wenn ungültige Parameter eingegeben werden
    if(n<0 || n > 7)
    {
        return false;
    }
    
    //wenn button gedrückt ist, ist verundung 0, also muss true zurückkommen, wenn gedrückt
   return ((GPIOF->IDR & (1 << n)) == 0);
}

void setLEDs(bool* leds)
{
    for(int i = 0; i<8; i++)
    {
        //einschalten
        if(leds[i])
        {
            GPIOD->BSRR |= (1 << i);
        }
        //wenn nicht gedrückt lampe ausschalten
        else 
        {
            GPIOD->BSRR |= (1 << (i+16));
        }
    }
}