#include "../Inc/warten.h"
#include "stm32f4xx_hal.h"


//damit nur hier die sleep funktion geändert werden müsste, falls HAL_Delay nicht gut ist für sleep.
void warten(int millisekunden)
{
    HAL_Delay(millisekunden);
}