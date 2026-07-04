#include "calc.h"
#include "GPIO_read.h"
#include "error.h"
#include "zeitmessung.h"
#include <stdint.h>
#include "ISRs.h"

//lokale variablen
double winkel               = 0.0;
double geschwindigkeit      = 0.0;
bool fehlerBeiPhasenwechsel = false;
int richtung;

//variablen für die geschwindigkeitsberechnung -> static, um wert zwischen funktionen beizubehalten
static uint32_t t_start = 0;
static int32_t pulse_start = 0;

//für phasenberechnung für isr aufgabe
extern volatile Phase_t letztePhase;
extern volatile Phase_t aktuellePhase;
extern volatile int32_t phasenzahl;
extern volatile uint32_t letzter_zeitstempel;

//Für Zählen der Phasenwechsel
// alt: int phasenzahl;

//ALT:  //Zahlen aus GPIO_read -> statusDrehscheibe
// extern int kanal1;
// extern int kanal2;

//Initialisierung für Rechner -> zu beginn oder nach fehler löschen
void calcInit()
{
    // Startphase einlesen 
    //letztePhase                  = berechneAktuellePhase(); Das geht nicht, denn berechneAktuellePhase() gibt 0, also einen Statuscode, statt dessen:
    get_aktuelle_phase();
    letztePhase = aktuellePhase;

    richtung                     = UNBEKANNT;
    fehlerBeiPhasenwechsel       = false;
    phasenzahl                   = 0;
    t_start = 0;
    pulse_start = 0;
}

// int berechnePhasenwechsel2(void) // Ansatz, dass allles in calc ist
// {
//     if (aktuellePhase != letztePhase)
//     {
//         switch (letztePhase) 
//         {
//             case PHASE_B:
//                 if      (aktuellePhase == PHASE_A) {phasenzahl--; richtung = RUECKWAERTS;}
//                 else if (aktuellePhase == PHASE_C) {phasenzahl++; richtung = VORWAERTS;}
//                 else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;} // Phase B auf D
//                 letztePhase = aktuellePhase;
//                 break;

//             case PHASE_A:
//                 if      (aktuellePhase == PHASE_D) {phasenzahl--; richtung = RUECKWAERTS;}
//                 else if (aktuellePhase == PHASE_B)  {phasenzahl++; richtung = VORWAERTS;}
//                 else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
//                 letztePhase = aktuellePhase;
//                 break;
            
//             case PHASE_C:
//                 if      (aktuellePhase == PHASE_B) {phasenzahl--; richtung = RUECKWAERTS;}
//                 else if (aktuellePhase == PHASE_D) {phasenzahl++; richtung = VORWAERTS;}
//                 else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
//                 letztePhase = aktuellePhase;
//                 break;
            
//             case PHASE_D:
//                 if      (aktuellePhase == PHASE_C) {phasenzahl--; richtung = RUECKWAERTS;}
//                 else if (aktuellePhase == PHASE_A) {phasenzahl++; richtung = VORWAERTS;}
//                 else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
//                 letztePhase = aktuellePhase;
//                 break;

//             default:
//                 letztePhase = aktuellePhase;
//                 return -1; // Passt das? // lieber UNBEKANNTER_phasenwechsel_fehler
//                 break;
//         }
//     }
//     else richtung = GLEICH;

//     return 0;
// }

/*
int berechneWinkel(int phasenzahl, double* winkel)
{
    if (phasenzahl == 0) *winkel = 0;
    else *winkel = (1200.0 / 360.0) * *winkel; 
}
*/

void berechneGeschwindigkeit() 
{
    uint32_t sicherer_zeitstempel;
    uint32_t sichere_phasenzahl;

    //daten aus den isr-variablen holen
    get_drehgeberdaten(&sicherer_zeitstempel, &sichere_phasenzahl);
    
    //ist exakter Zeitpunkt, an dem der letzte Phasenwechsel stattfand
    uint32_t t_end = sicherer_zeitstempel;

    //zeitspanne berechnen
    double dt = timer_get_duration(t_start, t_end);

    //division durch 0 verhindern
    if(dt <= 0.0)
    {
        geschwindigkeit = 0.0;
        return;
    }

    //Phasenwechsel im Zeitfenster berechnen
    int32_t delta_pulse = (int32_t) sichere_phasenzahl - pulse_start;

    //winkeländerung in grad
    double delta_phi = (double)delta_pulse * GRAD_PRO_PHASE;

    //geschwindigkeit berechnen
    geschwindigkeit = delta_phi / dt;

    //werte für nächstes zeitfenster speichern
    t_start = t_end;
    pulse_start = (int32_t)sichere_phasenzahl;

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


//neu für isr
// Bestimmt die Phase "normal"
Phase_t get_aktuelle_phase(void) 
{
    // Knöpfe/Signale einlesen
    // Wenn Signal da (0V) -> bool ist false. Wenn Signal weg (3.3V) -> bool ist true.
    bool kanal1 = (GPIOG->IDR & (1 << 0)) ? true : false; // AUX0
    bool kanal2 = (GPIOG->IDR & (1 << 1)) ? true : false; // AUX1

    if      (!kanal1 && !kanal2)    return PHASE_A;
    else if (kanal1 && !kanal2)     return PHASE_B;
    else if (kanal1 && kanal2)      return PHASE_C;
    else                            return PHASE_D;
}

void verarbeite_phasenwechsel(uint32_t zeitstempel)
{
    //aktuellePhase = get_aktuelle_phase();

    if (aktuellePhase != letztePhase)
    {
        switch (letztePhase) 
        {
            case PHASE_B:
                if      (aktuellePhase == PHASE_A) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_C) {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;} // Phase B auf D
                letztePhase = aktuellePhase;
                break;

            case PHASE_A:
                if      (aktuellePhase == PHASE_D) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_B)  {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                letztePhase = aktuellePhase;
                break;
            
            case PHASE_C:
                if      (aktuellePhase == PHASE_B) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_D) {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                letztePhase = aktuellePhase;
                break;
            
            case PHASE_D:
                if      (aktuellePhase == PHASE_C) {phasenzahl--; richtung = RUECKWAERTS;}
                else if (aktuellePhase == PHASE_A) {phasenzahl++; richtung = VORWAERTS;}
                else    {richtung = phasenwechsel_fehler; fehlerBeiPhasenwechsel = true;}
                letztePhase = aktuellePhase;
                break;

            default:
                letztePhase = aktuellePhase;
                break;
        }
    }
    else richtung = GLEICH;


    letzter_zeitstempel = zeitstempel;
    letztePhase = aktuellePhase;
}