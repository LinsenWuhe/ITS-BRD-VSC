#include "calc.h"
#include "GPIO_read.h"
#include <stdbool.h>
#include "timer.h"
#include "GPIO_read.h"
#include "lcd.h"

#define PHASEN_PRO_UMDREHUNG 1200
#define GRAD_PRO_PHASE       (360.0/ PHASEN_PRO_UMDREHUNG)


int pulse_count;
bool error;
double winkel = 0.0;
double geschwindigkeit = 0.0;

//Initialisierung der letzten Phase und Richtung
phase_t letztePhase = PHASE_B;
richtung_t richtung = RICHTUNG_UNBEKANNT;

/* -------------------------------------------------------
 * calcInit
 * Initialisiert den Startzustand korrekt
 * -------------------------------------------------------*/
void calcInit(void)
{
    /* Echte Startphase einlesen statt PHASE_B anzunehmen */
    letztePhase = berechneAktuellePhase();
    pulse_count = 0;
    richtung    = RICHTUNG_UNBEKANNT;
    error       = false;
}


void berechneWinkel()
{
    winkel = pulse_count * (360.0/1200.0);
}

void berechneGeschwindigkeit(uint32_t t_start, uint32_t t_end, int32_t pulse_start)
{
    // Zeitspanne in Sekunden 
    double dt = timer_get_duration(t_start, t_end);

    // Sicherheit - Division durch 0 vermeiden 
    if (dt <= 0.0) {
        geschwindigkeit = 0.0;
        return;
    }

    // Phasenwechsel im Zeitfenster 
    int32_t delta_pulse = gibPulseCount() - pulse_start;

    // Winkelaenderung in Grad 
    double delta_phi = (double)delta_pulse * GRAD_PRO_PHASE;

    // Winkelgeschwindigkeit in Grad/Sekunde 
    geschwindigkeit = delta_phi / dt;
}
phase_t berechneAktuellePhase()
{
    //aktuelle Phase bestimmen
    if (kanal1 == 0 && kanal2 == 0) 
    {
        return PHASE_A;
    }
    else if(kanal1 == 1 && kanal2 == 0)
    {
        return PHASE_B;
    }
    else if(kanal1 == 1 && kanal2 == 1)
    {
        return PHASE_C;
    }
    else // else if (kanal1 == 0 && kanal2 == 1)
    {
        return PHASE_D;
    }
}

void berechnePhasenWechsel(void)
{
    phase_t aktuellePhase = berechneAktuellePhase();

    if (aktuellePhase == letztePhase) return;

    switch (letztePhase) {
        case PHASE_A:
            if      (aktuellePhase == PHASE_D) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_B) { pulse_count++; richtung = RICHTUNG_VOR; }
            else if (aktuellePhase == PHASE_C) { 
                error = true;
                /* DEBUG - welcher Übergang? */
                lcdGotoXY(6, 13);
                lcdPrintS("ERR: A->C");
            }
            break;

        case PHASE_B:
            if      (aktuellePhase == PHASE_A) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_C) { pulse_count++; richtung = RICHTUNG_VOR; }
            else if (aktuellePhase == PHASE_D) { 
                error = true;
                lcdGotoXY(6, 13);
                lcdPrintS("ERR: B->D");
            }
            break;

        case PHASE_C:
            if      (aktuellePhase == PHASE_B) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_D) { pulse_count++; richtung = RICHTUNG_VOR; }
            else if (aktuellePhase == PHASE_A) { 
                error = true;
                lcdGotoXY(6, 13);
                lcdPrintS("ERR: C->A");
            }
            break;

        case PHASE_D:
            if      (aktuellePhase == PHASE_C) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_A) { pulse_count++; richtung = RICHTUNG_VOR; }
            else if (aktuellePhase == PHASE_B) { 
                error = true;
                lcdGotoXY(6, 13);
                lcdPrintS("ERR: D->B");
            }
            break;

        default:
            error = true;
            lcdGotoXY(6, 13);
            lcdPrintS("ERR: DEFAULT");
            break;
    }

    letztePhase = aktuellePhase;
}

//Getter
int gibPulseCount(void)
{
    return pulse_count;
}

richtung_t gibRichtung(void)
{
    return richtung;
}
bool gibFehler(void)
{
    return error;
}

double gibWinkel(void)
{
    return winkel;
}

double gibGeschwindigkeit(void)
{
    return geschwindigkeit;
}


void fehlerLoeschen(void)
{
    error = false;
}




