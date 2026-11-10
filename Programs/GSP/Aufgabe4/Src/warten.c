#include "../Inc/warten.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include <stdint.h>


//damit nur hier die sleep funktion geändert werden müsste, falls HAL_Delay nicht gut ist für sleep.
void warten(int mikrosekunden)
{
    uint32_t start = getTimeStamp();
    uint32_t ticks_to_wait = mikrosekunden * TICKS_PER_US;

    //schleife läuft bis die differenz zwischen aktueller zeit und startzeit größer oder gleich den benötigten ticks ist
    while((getTimeStamp() - start) < ticks_to_wait)
    {
        //nichts tun, nur warten
    }

}