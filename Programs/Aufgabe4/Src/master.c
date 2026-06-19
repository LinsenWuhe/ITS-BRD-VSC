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



uint8_t berechneCRC(uint8_t *daten, int laenge)
{
    uint8_t crc = 0;

    //schleife über alle bytes im Array
    for (int i = 0; i < laenge; i++) 
    {
        uint8_t inbyte = daten[i];

        //Schleife über alle 8 Bits des aktuelle Bytes (von lsb nach msb)
        for (int b = 0; b<8; b++) 
        {
            //Prüfen, ob das untersete Bit des aktuelle CRC-Werts sich vom aktuellen Datenbit unterscheidet
            uint8_t mix = (crc ^inbyte) & 0x01;

            //CRC um eine Stelle nach rechts schieben
            crc >>= 1;

            //wernn der mix 1 war, wir das polynom (0x8C) per XOR verknüpft
            if (mix)
            {
                crc ^= 0x8C; //blatt an27 -> reflektiertes Polynom für X^8 + X^5 + X^4 + 1
            }

            //daten-byte weiterschieben für nächstes Bit
            inbyte >>= 1;
        }
    }

    return crc;
}

static uint8_t ROM_NO[8];
static int letzter_konflikt = 0;
static int suche_beendet = 0;

void starteSucheNeu() 
{
    letzter_konflikt = 0;
    suche_beendet = 0;
    for(int i = 0; i < 8; i++) ROM_NO[i] = 0;
}

// Sucht den nächsten Sensor auf dem Bus. Schreibt die ID in 'id_buffer'.
// Gibt 1 zurück, wenn einer gefunden wurde, 0 wenn die Suche vorbei ist.
int sucheNaechstenSensor(uint8_t *id_buffer) 
{
    int bit_index = 1;
    int aktueller_konflikt = 0;
    int byte_index = 0;
    uint8_t bit_maske = 1;
    int search_richtung = 0;
    
    if (suche_beendet) 
    {
        return 0;
    }
    if (reset() != OK)
    {
         starteSucheNeu(); return 0;
    }
    
    sendeByte(0xF0); // 0xF0 ist der offizielle "Search ROM" Befehl
    
    while (bit_index <= 64) 
    {
        int bit = liesBit();
        int bit_invertiert = liesBit();
        
        if (bit == 1 && bit_invertiert == 1)
        {
             break; //Fehler
        }               
        else if (bit != bit_invertiert) 
        {
            search_richtung = bit;
        }
        else { // Konflikt (Kreuzung im Binärbaum)
            if (bit_index < letzter_konflikt) 
            {
                search_richtung = ((ROM_NO[byte_index] & bit_maske) > 0) ? 1 : 0;
            } 
            else if (bit_index == letzter_konflikt)
            {
                search_richtung = 1;
            } 
            else 
            {
                search_richtung = 0;
            }
            if (search_richtung == 0) 
            {
                aktueller_konflikt = bit_index;
            }
        }
        
        if (search_richtung == 1) 
        {
            ROM_NO[byte_index] |= bit_maske;
        }
        else
        {
            ROM_NO[byte_index] &= ~bit_maske;
        }
        
        if (search_richtung == 1) 
        {
            sende1();
        }
        else 
        {
            sende0();
        }
        
        bit_index++;
        bit_maske <<= 1;

        if (bit_maske == 0) 
        { 
            byte_index++; bit_maske = 1; 
        }
    }
    
    letzter_konflikt = aktueller_konflikt;
    if (letzter_konflikt == 0) 
    {
        suche_beendet = 1;
    }
    
    for (int i = 0; i < 8; i++) 
    {
        id_buffer[i] = ROM_NO[i];
    }
    return 1;
}