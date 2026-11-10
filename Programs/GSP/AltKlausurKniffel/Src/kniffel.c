/**
 ******************************************************************************
 * @file    kniffel.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    20. Jan. 2025
 * @brief   Dieses Modul speichert den Zustand der W�rfel.
 *
 ******************************************************************************
 */

#include "kniffel.h"
#include "LCD_GUI.h"
#include "display.h"
#include "error.h"
#include "random.h"
#include <stdbool.h>
#include <stdint.h>

int wuerfelWerte[5];
bool fixedWerte[5];

uint8_t getDie(uint8_t die)
{
    switch (die) 
    {
        case 0:
            return wuerfelWerte[0];
        case 1:
            return wuerfelWerte[1];
        case 2:
            return wuerfelWerte[2];
        case 3:
            return wuerfelWerte[3];
        case 4: 
            return wuerfelWerte[4];
        default:
            Error_Handler();
            return 0;
    }
}

void druckeAlleGelb()
{
    for (int i = 0; i<5 ; i++) 
    {
        displayDie(i, wuerfelWerte[i], YELLOW);
    }
}

bool isKniffel()
{

    if (
        wuerfelWerte[0] == wuerfelWerte[1] 
        && wuerfelWerte[1] == wuerfelWerte[2]
        && wuerfelWerte[2] == wuerfelWerte[3]
        && wuerfelWerte[3] == wuerfelWerte[4]
    ) 
    {
        druckeAlleGelb();
        return true;
    }
    else
    {
        return false;
    }
}

bool einWuerfelHatZahl(int i)
{
    if (wuerfelWerte[0] == i || wuerfelWerte[1] == i || wuerfelWerte[2] == i || wuerfelWerte[3] == i || wuerfelWerte[4] == i) 
    {
        return true;
    }
    return false;
}

bool isStreet()
{
    if 
    (
        //entwender straße von 1-5
       (einWuerfelHatZahl(1) && einWuerfelHatZahl(2) && einWuerfelHatZahl(3) && einWuerfelHatZahl(4) && einWuerfelHatZahl(5))
       //oder straße von 2-6
       || (einWuerfelHatZahl(6) && einWuerfelHatZahl(2) && einWuerfelHatZahl(3) && einWuerfelHatZahl(4) && einWuerfelHatZahl(5))
    ) 
     
    {
        druckeAlleGelb();
        return true;
    }
    else 
    {
        return false;
    }
}

void fixDie(uint8_t die)
{
    displayDie(die, getDie(die), MAGENTA);
    fixedWerte[die] = true;
}

void releaseDice(void)
{
    displayDie(0, getDie(0), GREEN);
    displayDie(1, getDie(1), GREEN);
    displayDie(2, getDie(2), GREEN);
    displayDie(3, getDie(3), GREEN);
    displayDie(4, getDie(4), GREEN);

    for (int i = 0; i<5; i++) 
    {
        fixedWerte[i] = false;
    }
}

bool isFixed(uint8_t die)
{
    return fixedWerte[die];
}
int zufalligeZahlVon1Bis6(void)
{
    return (nxtRandNumber() % 6) +1;
}

void rollDice(void)
{
    clearDisplay();
    for(int i = 0; i<5; i++)
    {
        if (isFixed(i)) 
        {
            //würfel wird mit gleichem wert nochmal gedruckt, wird aber grün, damit er in der nächsten runde evtl mitgewürfelt werden kann
             displayDie((i), wuerfelWerte[i], GREEN);
            continue;
        }
        else 
        {
            displayDie((i), zufalligeZahlVon1Bis6(), GREEN);
        }
    }
}




// EOF