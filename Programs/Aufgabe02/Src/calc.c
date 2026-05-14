#include "calc.h"
#include "GPIO_read.h"


int pulse_count; //Zähler für Zustandswechsel
double winkel = 0.0;
double geschwindigkeit = 0.0;
bool fehlerBeiPhasenwechsel = false;
int richtung;

//für phasenberechnung
int letztePhase;
int aktuellePhase;

//auch in der main
int phasenzahl;

//Zahlen aus GPIO_read -> statusDrehscheibe
extern int kanal1;
extern int kanal2;

//Initialisierung für Rechner -> zu beginn oder nach fehler löschen
void calcInit()
{
    // Startphase einlesen 
    letztePhase = berechneAktuellePhase();
    pulse_count = 0;
    richtung    = UNBEKANNT;
    fehlerBeiPhasenwechsel       = false;
    phasenzahl = 0;
}

//Statt kanalA und kanalB, direkt auf kanal1 und kanal2 aus GPIO read zugreifen - alt: int berechneAktuellePhase(char kanalA, char kanalB, int* phase)
int berechneAktuellePhase(void)
{
    //status_drehscheibe() wird in der main eingelesen zu Beginn -> kanal1 und 2 werden gesetzt

    if (!kanal1 && !kanal2) aktuellePhase = PHASE_A;

    else if (kanal1 && !kanal2) aktuellePhase = PHASE_B;

    else if (kanal1 && kanal2) aktuellePhase = PHASE_C;

    else aktuellePhase = PHASE_D;

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
                if (aktuellePhase == PHASE_A) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_C) {phasenzahl++; *ergebnis = VORWAERTS;}
                else {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;} // Phase B auf D
                break;

            case PHASE_A:
                if (aktuellePhase == PHASE_D) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_B)  {phasenzahl++; *ergebnis = VORWAERTS;}
                else {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_C:
                if (aktuellePhase == PHASE_B) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_D) *ergebnis = VORWAERTS;
                else {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;
            
            case PHASE_D:
                if (aktuellePhase == PHASE_C) {phasenzahl--; *ergebnis = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_A) {phasenzahl++; *ergebnis = VORWAERTS;}
                else {*ergebnis = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                break;

            default:
                return -1; // Passt das? // lieber UNBEKANNTER_phasenwechsel_fehler
                break;
        }
    }
    else *ergebnis = GLEICH;

    return 0;
}

int berechneWinkel(void)
{
    winkel = pulse_count * GRAD_PRO_PHASE;
    return 0;
}


//Getter
double gibWinkel(void)
{
    return winkel;
}

int gibPulseCount(void)
{
    return pulse_count;
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



