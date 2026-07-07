#ifndef __CALC_H
#define __CALC_H

#include "error.h"
#include <stdbool.h>
#include <stdint.h>

// #define PHASE_A 10
// #define PHASE_B 20
// #define PHASE_C 30
// #define PHASE_D 40

// #define RUECKWAERTS 50
// #define GLEICH      60
// #define VORWAERTS   70
// #define UNBEKANNT   80

//Phasendefinitionen
typedef enum { PHASE_A, PHASE_B, PHASE_D, PHASE_C } Phase_t;
typedef enum { UNBEKANNT, VORWAERTS, RUECKWAERTS, GLEICH, PHASENWECHSEL_FEHLER } Richtung_t;

#define PHASEN_PRO_UMDREHUNG 1200
#define GRAD_PRO_PHASE       (360.0/ PHASEN_PRO_UMDREHUNG)


// int berechneAktuellePhase(void);

// int berechnePhasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis);
// int berechnePhasenwechsel2(void);

void berechneGeschwindigkeit(void);

//int berechneWinkel(int phasenzahl, double* winkel);
int berechneWinkel(void);

void calcInit(void);


//Getter
double gibWinkel(void);

int gibPulseCount(void);

double gibGeschwindigkeit(void);

bool gibFehler(void);

int gibRichtung(void);


//neu für isr
void verarbeite_phasenwechsel(uint32_t zeitstempel);
void verarbeite_phasenwechsel2();
Phase_t get_aktuelle_phase(void) ;

#endif