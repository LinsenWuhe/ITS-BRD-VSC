/* statusDrucken.c */
#include "util.h"
#include "GPIO_read.h"
#include "lcd.h"
#include "calc.h"

/* -------------------------------------------------------
 * statusDrucken
 * Gibt Winkel, Geschwindigkeit und Status auf LCD aus
 * Wird nur alle 250ms-500ms aufgerufen
 * -------------------------------------------------------*/
void statusDrucken(void)
{
    /* Werte aus den Modulen holen - kein extern noetig! */
    int winkel          = (int)gibWinkel();
    int geschwindigkeit = (int)gibGeschwindigkeit();


    /* Winkel ausgeben */
    lcdGotoXY(6, 7);
    lcdPrintS("Winkel:  ");
    lcdPrintInt(winkel);
    lcdPrintS(" Grad  ");       /* Leerzeichen um alte Zeichen zu ueberschreiben */

    /* Winkelgeschwindigkeit ausgeben */
    lcdGotoXY(6, 8);
    lcdPrintS("Tempo:   ");
    lcdPrintInt(geschwindigkeit);
    lcdPrintS(" Grad/s  ");

    /* Richtung ausgeben */
    lcdGotoXY(6, 9);
    lcdPrintS("Richtung: ");
    if (gibRichtung() == RICHTUNG_VOR) {
        lcdPrintS("vorwaerts ");
    }
    else if (gibRichtung() == RICHTUNG_ZUR) {
        lcdPrintS("rueckwaerts");
    }
    else {
        lcdPrintS("unbekannt  ");
    }

    /* Fehler ausgeben */
    lcdGotoXY(6, 11);
    if (gibFehler()) {
        lcdPrintS("Fehler: LIEGT VOR!");
    }
    else {
        lcdPrintS("Fehler: keine     ");
    }
}

void textInit(void)
{
lcdGotoXY(6, 4);
lcdPrintS("Drehgeber Auswertung");

lcdGotoXY(6, 6);
lcdPrintS("--------------------");

lcdGotoXY(6, 7);
lcdPrintS("Winkel:  ");

lcdGotoXY(6, 8);
lcdPrintS("Tempo:   ");

lcdGotoXY(6, 9);
lcdPrintS("Richtung:");

lcdGotoXY(6, 11);
lcdPrintS("Fehler:  ");

}