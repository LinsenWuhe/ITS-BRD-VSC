#include "stm32f4xx_hal.h"
#include "timer.h"
#include "../Inc/master.h"
#include "../Inc/errors.h"
#include "../Inc/warten.h"


/**
Um eine 0 zu senden wird er Bus 60 mikrosekunden auf low gezogen und dann wieder freigegeben und 10 mikrosekunden gewartet
*/
void sende0()
{
    GPIOG->BSRR = (1 << 16);
    warten(0.060);

    GPIOG->BSRR = 1;
    warten(0.010);
}

/**
Um eine 1 zu senden wird der Bus erst 6 mikrosekunden auf low gesetz und dann freigegeben und 64 mikrosekunden gewartet
*/
void sende1()
{
    GPIOG->BSRR = (1 << (PIN + 16));
    warten(0.006);

    GPIOG->BSRR = (1 << PIN);
    warten(0.064);
}


//bus auf low setzen, 6 ms warten, wieder freigeben 9 ms warten und bit vom bus lesen und 55 ms warten
int liesBit()
{
    GPIOG->BSRR = (1 << (PIN + 16));
    warten(0.006);
    GPIOG->BSRR = (1 << PIN);
    warten(0.009);

    int bit;
    /**
    bitweiser vergleich (nicht &&) -> wir vergleichen mit 00000001

        in Register xxxxxxxxxx
        unser Wert  0000000001
        ----------------------
       &         00000000 1/0 

       -> wenn 1: wird true und wenn 0 wird false (übersprungen)
     */
    if(GPIOG->IDR & (1 <<PIN))
    {
        bit = 1;
    }
    else
    {
        bit = 0;
    }

    warten(0.055);

    return bit;
}

//bus auf low setzen, 480 ms warten, freigeben und 70 ms warten, buszustand abfragen, 410 ms warten
int reset()
{
    GPIOG->BSRR = (1 << (16+ PIN));
    warten(0.480);
    GPIOG->BSRR = (1 << PIN);
    warten(0.07);

    int status;

    if(liesBit() == 0)
    {
        status = OK;
    }
    else
    {
        status = KEIN_TEILNEHMER;
    }

    warten(0.410);

    return status;
}

