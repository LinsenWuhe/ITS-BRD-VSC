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

#define PHASEN_PRO_UMDREHUNG 1200
#define GRAD_PRO_PHASE       (360.0/ PHASEN_PRO_UMDREHUNG)

int berechneAktuellePhase(char kanalA, char kanalB, int* phase);

int berechnePhasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis);

int berechneWinkel(void);


//Getter
double gibWinkel(void);

int gibPulseCount(void);

#endif