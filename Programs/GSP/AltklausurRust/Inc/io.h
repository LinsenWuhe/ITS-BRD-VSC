#ifndef IO_H
#define IO_H

#include <stdbool.h>

/**
* Liefert true zurück, wenn Taster n gedrügt ist
* @param n Der abzufragende Button; muss zwischen 0 und 7 sein
*/
bool buttonPressed(int n);

/**
* Steuer die Leds D8 bis D15. 
* @param leds ist ein Array mit (maximal) 8 Einträgen. Ist led[i] == true, wird die entsprechende LED gesetzt, bzw ausgeschaltet.
*/
void setLEDs(bool* leds);


#endif