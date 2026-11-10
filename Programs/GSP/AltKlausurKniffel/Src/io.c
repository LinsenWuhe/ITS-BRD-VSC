#include "io.h"
#include "error.h"
#include "stm32f429xx.h"
#include <stdint.h>

void setLEDs(uint8_t noLeds)
{
    uint32_t maske = 0b11111111;

    if(noLeds > 16)
    {
        Error_Handler();
    }

    //unteren teil anschalten
    if(noLeds < 9)
    {
        //alle LEDs ausschalten
        GPIOD->BSRR = maske << 16;
        GPIOE->BSRR = maske << 16;

        GPIOD->BSRR = maske >> (8-noLeds);
    }
    //oberen teil anschalten
    if (noLeds > 8) 
    {
        noLeds -= 8;

        GPIOE->BSRR = maske << 16;
        GPIOD->BSRR = maske;
        GPIOE->BSRR = maske >> (8-noLeds);
    }

}

bool buttonPressed(uint8_t button)
{
    if(button > 7)
    {
        Error_Handler();
    }

    return !(GPIOF->IDR & (1 << button));
}

// EOF