#ifndef __GPIO_READ_H
#define __GPIO_READ_H

#include "error.h" //ohne gibt es einen Fehler in der c-Datei bei den error.h macros

//wenn Spannung erkannt wird oder nicht
#define HIGH    1
#define LOW     0

//Für Knopf S6
#define s6_gedrueckt        1
#define s6_nicht_gedrueckt  0


//Zahlen aus C datei - für Übersicht
extern int kanal1;
extern int kanal2;

void status_drehscheibe_isr();
int status_drehscheibe_aufgabe2(void); //liest den aktuellen Status der Drehscheibe ein -> wird in kanal1 & kanal2 gespeichert
int s6_lesen(void);           //liest den aktuellen Status von S6 ein

#endif