#include "util.h"
#include "GPIO_read.h"
#include "lcd.h"
#include "calc.h"

extern int aktuellePhase;

void statusDrucken(void)
{
    extern char kanal1;
    extern char kanal2;

    //Kanal1 ausgeben
    lcdGotoXY(6, 4);
    lcdPrintS("Kanal 1: ");
    lcdPrintC(kanal1);
   
    //Kanal2 ausgeben
    lcdGotoXY(6, 5);
    lcdPrintS("Kanal 2: ");
    lcdPrintC(kanal2);   

    //Winkel ausgeben
    lcdGotoXY(6, 7);
    lcdPrintS("Winkel: ");
    lcdPrintInt(123);
    lcdPrintS(" Grad");

    //Winkelgeschwindigkeit ausgeben
    lcdGotoXY(6, 8);
    lcdPrintS("Tempo: ");
    lcdPrintInt(90);
    lcdPrintS(" Grad / s");

    lcdGotoXY(6, 10);
    lcdPrintS("Phase: ");
    lcdPrintInt(aktuellePhase);

    lcdGotoXY(6, 11);
    lcdPrintS("Fehler: keine");
}