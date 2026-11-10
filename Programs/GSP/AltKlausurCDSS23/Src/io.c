
#include "io.h"
#include "error.h"
#include "stm32f429xx.h"

uint32_t maske = 0b11111111;

void setLEDs(int noLeds)
{
    if (noLeds < 0 || noLeds > 16) 
    {
        Error_Handler();
    }

    //alle Leds ausschalten
    GPIOE->BSRR = (maske << 16);
    GPIOD->BSRR = (maske << 16);

    //untere Leds einschalten
    GPIOD->BSRR = ~(maske << noLeds);

    //obere leds einschalten
    if (noLeds > 8) 
    {
    noLeds -= 8;
    GPIOE->BSRR = ~(maske << noLeds);
    }

}