#include "text_output.h"
#include "lcd.h"
#include "stm32f429xx.h"
#include "calc.h"

 //statusDrucken-  Gibt Winkel, Geschwindigkeit und Status auf LCD aus - Wird nur alle 250ms-500ms aufgerufen
void statusDrucken(void)
{
    //Pin PG2 HIGH - Messung startet
    GPIOG -> MODER |= (1 << 2);
    GPIOG->ODR |= (1 << 2);

    // Werte aus den Modulen holen 
    int winkel          = (int)gibWinkel(); //gibWinkel() ist hier richtig !!!
    int geschwindigkeit = (int)gibGeschwindigkeit();


    // Winkel ausgeben 
    lcdGotoXY(16, 7);
    //lcdPrintS("Winkel:  ");
    lcdPrintInt(winkel);
    //lcdPrintS(" Grad  ");       

    // Winkelgeschwindigkeit ausgeben 
    lcdGotoXY(16, 8);
    //lcdPrintS("Tempo:   ");
    lcdPrintInt(geschwindigkeit);
    //lcdPrintS(" Grad/s  ");

    /*
    // Richtung ausgeben 
    lcdGotoXY(16, 9);
    //lcdPrintS("Richtung: ");
    if (gibRichtung() == VORWAERTS) {
        lcdPrintS("vorwaerts ");
    }
    else if (gibRichtung() == RUECKWAERTS) {
        lcdPrintS("rueckwaerts");
    }
    else {
        lcdPrintS("unbekannt  ");
    } 
    */

    /*
    // Fehler ausgeben 
    lcdGotoXY(6, 11);
    if (gibFehler()) {
        lcdPrintS("Fehler!");
    }
   /* else {
        lcdPrintS("Fehler: keine     ");
    }
    */
    

    //Pin LOW - Messung endet
    GPIOG -> ODR &= ~(1<<2);
}

void textInit(void)
{
lcdGotoXY(6, 4);
lcdPrintS("Drehgeber Auswertung");

lcdGotoXY(6, 6);
lcdPrintS("--------------------");

lcdGotoXY(6, 7);
lcdPrintS("Winkel:  ");

lcdGotoXY(19, 7);
lcdPrintS("Grad");

lcdGotoXY(6, 8);
lcdPrintS("Tempo:   ");
lcdGotoXY(19, 8);
lcdPrintS("Grad/s");

/*
lcdGotoXY(6, 9);
lcdPrintS("Richtung:");

lcdGotoXY(6, 11);
lcdPrintS("Fehler:  ");
*/

}