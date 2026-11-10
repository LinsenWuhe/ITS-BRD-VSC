#ifndef HILFSFUNKTIONEN_H
#define HILFSFUNKTIONEN_H

//setzt das gesamte Spiel zurück und ist bereit für ein neues Spiel
#include <stdbool.h>
void allesReset();

//zeigt die spielzüge auf den LEDs an
void ledAusgabe();

//liest alle GPIO pins ein
void liesGPIOein();

//verarbeitet die funktionen der Buttons
void verarbeiteGPIOs();

//prüft ob strasse oder kniffel gewürfelt wurde
void pruefeGewinn();

//prüft, ob noch gewürfelt werden kann
bool kannNochGewuerfeltWerden();

#endif