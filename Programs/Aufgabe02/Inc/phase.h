#ifndef __PHASENWECHSEL_H
#define __PHASENWECHSEL_H

#define PHASE_A 10
#define PHASE_B 20
#define PHASE_C 30
#define PHASE_D 40

#define RUECKWAERTS 50
#define GLEICH      60
#define VORWAERTS   70
#define FEHLER      80

int phase(char kanalA, char kanalB, int* phase);

int phasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis);



#endif