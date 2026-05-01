#ifndef CALC_H
#define CALC_H

#include <stdbool.h>
#include <stdint.h>
typedef enum {
    PHASE_A = 0,
    PHASE_B = 1,
    PHASE_C = 2,
    PHASE_D = 3,
} phase_t;

//Drehrichtung
typedef enum{
    RICHTUNG_UNBEKANNT = 0,
    RICHTUNG_VOR = 1,
    RICHTUNG_ZUR = 2,
} richtung_t;

void calcInit(void);
void berechneWinkel(void);

void berechneGeschwindigkeit(uint32_t t_start, uint32_t t_end, int32_t pulse_start);

phase_t berechneAktuellePhase();

void berechnePhasenWechsel();

//Getter
int gibPulseCount(void);
richtung_t gibRichtung(void);
bool gibFehler(void);
double gibWinkel(void);
double gibGeschwindigkeit(void);


void fehlerLoeschen(void);

#endif