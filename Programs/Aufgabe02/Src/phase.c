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
int phasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis)
{
    if (aktuellePhase != letztePhase)
    {
        switch (letztePhase) 
        {
            case PHASE_B:
                if (aktuellePhase == PHASE_A) *ergebnis = RUECKWAERTS;
                else if (aktuellePhase == PHASE_C) *ergebnis = VORWAERTS;
                else *ergebnis = FEHLER; // Phase B auf D
                break;

            case PHASE_A:
                if (aktuellePhase == PHASE_D) *ergebnis = RUECKWAERTS;
                else if (aktuellePhase == PHASE_B) *ergebnis = VORWAERTS;
                else *ergebnis = FEHLER;
                break;
            
            case PHASE_C:
                if (aktuellePhase == PHASE_B) *ergebnis = RUECKWAERTS;
                else if (aktuellePhase == PHASE_D) *ergebnis = VORWAERTS;
                else *ergebnis = FEHLER;
                break;
            
            case PHASE_D:
                if (aktuellePhase == PHASE_C) *ergebnis = RUECKWAERTS;
                else if (aktuellePhase == PHASE_A) *ergebnis = VORWAERTS;
                else *ergebnis = FEHLER;
                break;

            default:
                return -1; // Passt das? // lieber UNBEKANNTER_FEHLER
                break;
        }
    }
    else *ergebnis = GLEICH;

    return 0;
}