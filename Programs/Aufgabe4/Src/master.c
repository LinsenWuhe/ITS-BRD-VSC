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

//behalten ihren Wert bei, auch wenn die FUnktion sucheNaechstenSensor verlassen wird -> gedächtnis der Suche
static int letzter_konflikt = 0; //position 1-64 merkt er sich - 0 hier: (noch) kein konflikt
static int suche_beendet = 0; //wenn alle Sensoren gefunden wurden wird hier 1 hingeschrieben

//reset für suchalgorithmus -> fängt beim nächsten mal wieder von vorne an
void starteSucheNeu() 
{
    letzter_konflikt = 0;
    suche_beendet = 0;
    //array was zusammengesetzt wurde hier löschen
    for(int i = 0; i < 8; i++) 
    { 
        ROM_NO[i] = 0;
    }
}

// Sucht den nächsten Sensor auf dem Bus. Schreibt die ID in 'id_buffer'.
// Gibt 1 zurück, wenn einer gefunden wurde, 0 wenn die Suche vorbei ist.
//master geht alle 64 bits der id durch und löst konflikte schritt für schritt auf
int sucheNaechstenSensor(uint8_t *id_buffer) 
{
    //Vorbereitungen und Prüfungen
    
    int bit_index = 1;          //zähler für die 64 bits (von 1-64)
    int aktueller_konflikt = 0; //merkt sich den letzten Konflut in DIESEM durchlauf
    int byte_index = 0;         //welches der 8 bytes bearbeiten wir gerade?
    uint8_t bit_maske = 1;      //bitmaske, um das aktuelle Bit im byte zu setzen
    int search_richtung = 0;    //gehen wir in Richtung 0 oder 1 im Baum?
    
    if (suche_beendet) 
    {
        return 0; //wenn im letzten Durchlauf alle Sensoren gefunden wurden, wird abgebrochen
    }
    if (reset() != OK)
    {
         starteSucheNeu(); 
         return 0; //wenn Bus leer ist, wird suche zurückgesetzt
    }
    
    sendeByte(search_ROM); //alle Sensoren lauschen nun
    
    while (bit_index <= 64) //durchlaufen für jedes Bit der ID
    {
        int bit = liesBit();                //alle Sensoren senden ihr bit 
        int bit_invertiert = liesBit();     //und ihr Komplement
        
        if (bit == 1 && bit_invertiert == 1)
        {
             break; //Fehler, weil kein Sensor antwortet
        }               
        else if (bit != bit_invertiert) 
        {
            search_richtung = bit; //wenn zb bit=0 und bit_invertiert = 1 ist, dann haben ALLE angeschlossenen Sensoren an dieser Stelle eine 0 -> Richtung steht fest
        }
        else { // Konflikt (Kreuzung im Binärbaum), weil bit=0 und komplement = 0 - ein Sensor hat hier eine 0 und einer eine 1
            if (bit_index < letzter_konflikt) 
            {
                //wenn wir noch weiter oben im Baum sind als beim letzten Durchlauf, nehmen wir die Richtung, die wir in der Vergangenheit schon gewählt haben
                search_richtung = ((ROM_NO[byte_index] & bit_maske) > 0) ? 1 : 0;
            } 
            else if (bit_index == letzter_konflikt)
            {
                //wir sind exakt an der Kreuzung angekommen, die wir beim lezten Mal.
                //letztes Mal 0 -> jetzt 1
                search_richtung = 1;
            } 
            else 
            {
                //neuer Konflikt, den wir noch nicht gesehen haben
                search_richtung = 0;
            }
            if (search_richtung == 0) 
            {
                //wenn wir 0 Pfad genommen haben, merken wir uns die Position, damit beim nächsten Aufuruf der Funktion klar ist, wo man lang geht
                aktueller_konflikt = bit_index;
            }
        }
        
        //ermittelte Bit in zwischenspeicher ROM-NO speichern
        if (search_richtung == 1) 
        {
            //bit auf 1 setzen
            ROM_NO[byte_index] |= bit_maske;
        }
        else
        {
            //bit auf 0 setzen
            ROM_NO[byte_index] &= ~bit_maske;
        }
        
        //bit zurück an den Bus senden - alle sensoren, die NICHT diese RIchtung haben, schalten sich ab
        if (search_richtung == 1) 
        {
            sende1();
        }
        else 
        {
            sende0();
        }
        
        //zähler und masken für das nächste Bit vorbereiten
        bit_index++;
        // Maske um eins nach links schieben (z.B. von 00000001 zu 00000010)
        bit_maske <<= 1;

        //wenn Maske überläuft, ist Byte voll - wechlse zu nächsten byte
        if (bit_maske == 0) 
        { 
            byte_index++; bit_maske = 1; 
        }
    }
    //wir merken uns den letzten unaufgelösten Konflikt für den nächsten funktionsaufruf
    letzter_konflikt = aktueller_konflikt;

    //wenn es in diesem ganzen durchlauf überhaupt keienn konflikt mehr gibt: wir haben den letzten Sensor auf dem Bus gelesen, suche ist vorbei 
    if (letzter_konflikt == 0) 
    {
        suche_beendet = 1;
    }
    
    //die gefundene 64-bit-id in den id buffer des Aufrufers kopieren
    for (int i = 0; i < 8; i++) 
    {
        id_buffer[i] = ROM_NO[i];
    }
    //erfolg - 1 sensor wurde gelensen
    return 1;
}