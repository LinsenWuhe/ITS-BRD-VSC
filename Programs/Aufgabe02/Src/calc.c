#include "calc.h"
#include "GPIO_read.h"
#include <stdbool.h>
#include "timer.h"

#define PHASEN_PRO_UMDREHUNG 1200
#define GRAD_PRO_PHASE       (360.0/ PHASEN_PRO_UMDREHUNG)

extern int kanal1;
extern int kanal2;

int pulse_count;
bool error;
double winkel = 0.0;
double geschwindigkeit = 0.0;

//Initialisierung der letzten Phase und Richtung
phase_t letztePhase = PHASE_B;
richtung_t richtung = RICHTUNG_UNBEKANNT;

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
    else if(kanal1 == 0 && kanal2 == 1)
    {
        return PHASE_D;
    }
}

void berechnePhasenWechsel()
{   
    phase_t aktuellePhase = berechneAktuellePhase();

    switch (letztePhase) {

        case PHASE_A:
            if      (aktuellePhase == PHASE_D) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_B) { pulse_count++; richtung = RICHTUNG_VOR;  }
            else if (aktuellePhase == PHASE_C) { error = true;                            }
            // PHASE_A -> PHASE_A: keine Aenderung 
            break;

        case PHASE_B:
            if      (aktuellePhase == PHASE_A) { pulse_count--; richtung = RICHTUNG_VOR; }
            else if (aktuellePhase == PHASE_C) { pulse_count++; richtung = RICHTUNG_VOR;  }
            else if (aktuellePhase == PHASE_D) { error = true;                            }
            // PHASE_B -> PHASE_B: keine Aenderung 
            break;

        case PHASE_C:
            if      (aktuellePhase == PHASE_B) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_D) { pulse_count++; richtung = RICHTUNG_VOR;  }
            else if (aktuellePhase == PHASE_A) { error = true;                            }
            // PHASE_C -> PHASE_C: keine Aenderung 
            break;

        case PHASE_D:
            if      (aktuellePhase == PHASE_C) { pulse_count--; richtung = RICHTUNG_ZUR; }
            else if (aktuellePhase == PHASE_A) { pulse_count++; richtung = RICHTUNG_VOR;  }
            else if (aktuellePhase == PHASE_B) { error = true;                            }
            // PHASE_D -> PHASE_D: keine Aenderung 
            break;

        default:
            error = true;
            break;
    }

    // Letzte Phase fuer naechsten Durchlauf speichern 
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




