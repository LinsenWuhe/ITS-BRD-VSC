#include "calc.h"
#include "GPIO_read.h"


double winkel               = 0.0;
double geschwindigkeit      = 0.0;
bool fehlerBeiPhasenwechsel = false;
int richtung;

//für phasenberechnung
int letztePhase;
int aktuellePhase;

//Für Zählen der Phasenwechsel
int phasenzahl;

//Zahlen aus GPIO_read -> statusDrehscheibe
extern int kanal1;
extern int kanal2;

//Initialisierung für Rechner -> zu beginn oder nach fehler löschen
void calcInit()
{
    // Startphase einlesen 
    //letztePhase                  = berechneAktuellePhase(); Das geht nicht, denn berechneAktuellePhase() gibt 0, also einen Statuscode, statt dessen:
    berechneAktuellePhase();
    letztePhase = aktuellePhase;
    
    phasenzahl                   = 0;
    richtung                     = UNBEKANNT;
    fehlerBeiPhasenwechsel       = false;
    phasenzahl                   = 0;
}

//Statt kanalA und kanalB, direkt auf kanal1 und kanal2 aus GPIO read zugreifen - alt: int berechneAktuellePhase(char kanalA, char kanalB, int* phase)
int berechneAktuellePhase(void)
{
    //status_drehscheibe() wird in der main eingelesen zu Beginn -> kanal1 und 2 werden gesetzt

    if      (!kanal1 && !kanal2)    aktuellePhase = PHASE_A;

    else if (kanal1 && !kanal2)     aktuellePhase = PHASE_B;

    else if (kanal1 && kanal2)      aktuellePhase = PHASE_C;

    else                            aktuellePhase = PHASE_D;

    return 0;
}

// TODO: vervollständigen -- das ist schon vollständig? oder? -> Noch Zähler für Phasenzahl hinzugefügt
int berechnePhasenwechsel(int aktuellePhase, int letztePhase, int* ergebnis)
{
    if (aktuellePhase != letztePhase)
    {
        switch (letztePhase) 
        {
            case PHASE_B:
                if      (aktuellePhase == PHASE_A) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_C) {phasenzahl++; *ergebnis = VORWAERTS;}
                else    {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;} // Phase B auf D
                break;

            case PHASE_A:
                if      (aktuellePhase == PHASE_D) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_B)  {phasenzahl++; *ergebnis = VORWAERTS;}
                else    {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_C:
                if      (aktuellePhase == PHASE_B) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_D) *ergebnis = VORWAERTS;
                else    {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_D:
                if      (aktuellePhase == PHASE_C) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_A) {phasenzahl++; *ergebnis = VORWAERTS;}
                else    {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;

            default:
                return -1; // Passt das? // lieber UNBEKANNTER_phasenwechsel_fehler
                break;
        }
    }
    else *ergebnis = GLEICH;

    return 0;
}

int berechnePhasenwechsel2(void) // Ansatz, dass allles in calc ist
{
    if (aktuellePhase != letztePhase)
    {
        switch (letztePhase) 
        {
            case PHASE_B:
                if      (aktuellePhase == PHASE_A) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_C) {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;} // Phase B auf D
                break;

            case PHASE_A:
                if      (aktuellePhase == PHASE_D) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_B)  {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_C:
                if      (aktuellePhase == PHASE_B) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_D) richtung = VORWAERTS;
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_D:
                if      (aktuellePhase == PHASE_C) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_A) {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;

            default:
                return -1; // Passt das? // lieber UNBEKANNTER_phasenwechsel_fehler
                break;
        }
    }
    else richtung = GLEICH;

    return 0;
}

/*
int berechneWinkel(int phasenzahl, double* winkel)
{
    if (phasenzahl == 0) *winkel = 0;
    else *winkel = (1200.0 / 360.0) * *winkel; 
}
*/

int berechneGeschwindigkeit() // wie? anzahlphasenwechsel durch zeit? oder winkel durch zeit, ja oder?
{

}
int berechneWinkel(void)
{
    winkel = phasenzahl * GRAD_PRO_PHASE;
    return 0;
}


//Getter
double gibWinkel(void)
{
    return winkel;
}

int gibPulseCount(void)
{
    return phasenzahl;
}

double gibGeschwindigkeit(void)
{
    return geschwindigkeit;
}

bool gibFehler()
{
    return fehlerBeiPhasenwechsel;
}

int gibRichtung()
{
    return richtung;
}



