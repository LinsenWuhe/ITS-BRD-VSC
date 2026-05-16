#ifndef __CALC_H
#define __CALC_H

#include "error.h"

#define PHASE_A 10
#define PHASE_B 20
#define PHASE_C 30
#define PHASE_D 40

#define RUECKWAERTS 50
#define GLEICH      60
#define VORWAERTS   70

int berechneAktuellePhase(char kanalA, char kanalB, int* phase);

int berechnePhasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis);

int berechneWinkel(int phasenzahl, double* winkel);

#endif