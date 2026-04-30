#include "GPIO_read.h"
#include "stm32f429xx.h"

char kanal1;
char kanal2;


int status_drehscheibe()
{
    // Auslesen von PF0
    if (GPIOF->IDR & (1 << 0)) {
        // Pin PF0 ist HIGH
        kanal1 = '1';
    } else {
        // Pin PF0 ist LOW (0V)
        kanal1 = '0';
    }

    // Auslesen von PF1
    if (GPIOF->IDR & (1 << 1)) {
        // Pin PF1 ist HIGH
        kanal2 = '1';
    }
    else{
        kanal2 = '0';
    }

    return 0;
}