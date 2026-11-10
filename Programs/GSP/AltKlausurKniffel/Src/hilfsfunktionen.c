#include "display.h"
#include "io.h"
#include "kniffel.h"
#include "lcd.h"
#include "hilfsfunktionen.h"

int wuerfe;
bool buttonsPressed[8];
bool gewonnen = false;
extern bool fixedWerte[5];


void allesReset()
{
    setLEDs(0);
    clearDisplay();
    wuerfe = 0;
    lcdGotoXY(1, 8);
    lcdPrintS("Bereit fuer neues Spiel, s6 zum Starten");
    gewonnen = false;
    
        for (int i = 0; i<5; i++) 
    {
        fixedWerte[i] = false;
    }
}

void liesGPIOein()
{
    for (int i = 0; i<8; i++) 
    {
        buttonsPressed[i] = buttonPressed(i);
    }
}

void ledAusgabe()
{
    setLEDs(wuerfe);
}

void verarbeiteGPIOs()
{
    if (buttonsPressed[7]) 
    {
        allesReset();
    }


    if(buttonsPressed[5])
    {
        releaseDice();
    }

    for (int i = 0; i<5; i++) 
    {
        if (buttonsPressed[i]) 
        {
            fixDie(i);
        }
    }

    if (buttonsPressed[6]) 
    {
        rollDice();
        wuerfe++;
    }
}

void pruefeGewinn()
{
    //nur prüfen, wenn überhaupt schon gewürfelt wurdde
    if (wuerfe > 0) 
    {
       gewonnen = isStreet();
        gewonnen = isKniffel();    
    }

    if (gewonnen) 
    {
        lcdGotoXY(1, 8);
        lcdPrintS("Gewonnen! S7 zum Neustarten");
        //nichts tun, solange neues Spiel nicht gestartet wurde
        while (!buttonPressed(7)) 
        {
        
        }
    }
}

bool kannNochGewuerfeltWerden()
{
    if (wuerfe < 16) 
    {
        return true;
    }

    lcdGotoXY(1, 8);
    lcdPrintS("Maximale Wurfanzahl erreicht, bitte mit s6 neues Spiel starten");
    while (! buttonPressed(6)) {
    
    }
    return false;
}

