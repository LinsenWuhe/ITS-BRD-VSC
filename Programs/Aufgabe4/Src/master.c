#include "stm32f4xx_hal.h"
#include "timer.h"
#include "../Inc/master.h"
#include "../Inc/errors.h"
#include "../Inc/warten.h"
#include <stdint.h>


/**
Um eine 0 zu senden wird er Bus 60 mikrosekunden auf low gezogen und dann wieder freigegeben und 10 mikrosekunden gewartet
*/
void sende0()
{
    GPIOD->BSRR = (1 << 16);        //Bus auf low
    warten(60);

    GPIOD->BSRR =  (1 << 0);                //Bus freigeben
    warten(10);
}

/**
Um eine 1 zu senden wird der Bus erst 6 mikrosekunden auf low gesetz und dann freigegeben und 64 mikrosekunden gewartet
*/
void sende1()
{
    GPIOD->BSRR = (1 << 16);    //bus auf LOW
    warten(6);

    GPIOD->BSRR = (1 << 0);           //bus freigeben
    warten(64);     
}


//bus auf low setzen, 6 ms warten, wieder freigeben 9 ms warten und bit vom bus lesen und 55 ms warten
int liesBit()
{
    int bit = 0;

    GPIOD->BSRR = (1 << 16);        //bus auf Low
    warten(6);         //
    GPIOD->BSRR = (1 << 0);                   //bus freigeben
    warten(9);

    /**
    bitweiser vergleich (nicht &&) -> wir vergleichen mit 00000001

        in Register xxxxxxxxxx
        unser Wert  0000000001
        ----------------------
       &         00000000 1/0 

       -> wenn 1: wird true und wenn 0 wird false (übersprungen)
     */
    if(GPIOD->IDR &  (1 << 0))
    {
        bit = 1;
    }

    warten(55);

    return bit;
}

//bus auf low setzen, 480 ms warten, freigeben und 70 ms warten, buszustand abfragen, 410 ms warten
int reset()
{
    int status; 

    GPIOD->BSRR = (1 <<16);
    warten(480);
    GPIOD->BSRR =  (1 << 0);
    warten(70);

    if((GPIOD->IDR &1) == 0)
    {
        status = OK;
    }
    else
    {
        status = KEIN_TEILNEHMER;
    }

    warten(410);

    return status;
}

//bit für bit prüfen, um ganzes byte zu senden
void sendeByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        //wir prüfen nur das unterste Bit
        if(byte & 0x01)
        {
            sende1();
        }
        else 
        {
            sende0();
        }

        //byte um eine stelle nach rechts schieben, damit im nächsten durchlauf das nächste Bit im Lsb steht
        byte >>= 1;
    }
}

uint8_t liesByte()
{
    uint8_t byte = 0; //anfangen mit 00000000

    for(int i = 0; i< 8; i++)
    {
        if(liesBit() == 1)
        {
            //wenn das gelesene Bit eine 1 ist, schieben wir eine 1 an die i-te stelle und setzen sie in unsere byte variable
            byte |= (1<<i);
        }

        //wenn es eine ß ist, müssen wir nichts tun, da byte dort eh 0 ist
    }

    return byte;
}