#include "GPIO_read.h"
#include "stm32f429xx.h"

int kanal1;
int kanal2;


int status_drehscheibe()
{
    // Auslesen von PF0
    if (GPIOF->IDR & (1 << 0)) {
        // Pin PF0 ist HIGH
        kanal1 = HIGH;
    } else {
        // Pin PF0 ist LOW (0V)
        kanal1 = LOW;
    }

    // Auslesen von PF1
    if (GPIOF->IDR & (1 << 1)) {
        // Pin PF1 ist HIGH
        kanal2 = HIGH;
    }
    else{
        kanal2 = LOW;
    }

    return 0;
}

int s6_lesen(void)
{
    if (GPIOF->IDR & (1 << 6)) {
        return 1;  /* Taste gedrueckt */
    }
    return 0;
}