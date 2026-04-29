#include "phase.h"


int phase(char kanalA, char kanalB, int* phase)
{
    if (!kanalA && !kanalB) *phase = PHASE_A;

    else if (kanalA && !kanalB) *phase = PHASE_B;

    else if (kanalA && kanalB) *phase = PHASE_C;

    else *phase = PHASE_D;

    return 0;
}

// TODO: vervollständigen
int phasenwechsel(int aktuellePhase, int* ergebnis)
{
    if (aktuellePhase != letztePhase)
    {
        switch (letztePhase) 
        {
            // TODO: andere Phasen müssen auch implementiert werden
            case PHASE_B:
                if (aktuellePhase == PHASE_A) *ergebnis = RUECKWAERTS;
                else if (aktuellePhase == PHASE_C) *ergebnis = VORWAERTS;
                else *ergebnis = FEHLER;
                break;

            default:
                break;
        }
    }
    else *ergebnis = GLEICH;

    return 0;
}