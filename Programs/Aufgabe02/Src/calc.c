#include "calc.h"
#include "GPIO_read.h"

extern int kanal1;
extern int kanal2;

char aktuellePhase;
//für initialisierung
char letztePhase = 'b';


void berechneWinkel()
{

}

void berechneGeschwindigkeit()
{

}

void berechneAktuellePhase()
{
    //aktuelle Phase bestimmen
    if (kanal1 == 0 && kanal2 == 0) 
    {
        aktuellePhase = 'a';
    }
    else if(kanal1 == 1 && kanal2 == 0)
    {
        aktuellePhase = 'b';
    }
    else if(kanal1 == 1 && kanal2 == 1)
    {
        aktuellePhase = 'c';
    }
    else if(kanal1 == 0 && kanal2 == 1)
    {
        aktuellePhase = 'b';
    }
}

void berechnePhasenWechsel()
{

    /*
    
    switch (last_phase) {

        case PHASE_A:
            if      (current_phase == PHASE_D) { pulse_count--; direction = DIR_BACKWARD; }
            else if (current_phase == PHASE_B) { pulse_count++; direction = DIR_FORWARD;  }
            else if (current_phase == PHASE_C) { error = true;                            }
            // PHASE_A -> PHASE_A: keine Aenderung 
            break;

        case PHASE_B:
            if      (current_phase == PHASE_A) { pulse_count--; direction = DIR_BACKWARD; }
            else if (current_phase == PHASE_C) { pulse_count++; direction = DIR_FORWARD;  }
            else if (current_phase == PHASE_D) { error = true;                            }
            // PHASE_B -> PHASE_B: keine Aenderung *
            break;

        case PHASE_C:
            if      (current_phase == PHASE_B) { pulse_count--; direction = DIR_BACKWARD; }
            else if (current_phase == PHASE_D) { pulse_count++; direction = DIR_FORWARD;  }
            else if (current_phase == PHASE_A) { error = true;                            }
            // PHASE_C -> PHASE_C: keine Aenderung *
            break;

        case PHASE_D:
            if      (current_phase == PHASE_C) { pulse_count--; direction = DIR_BACKWARD; }
            else if (current_phase == PHASE_A) { pulse_count++; direction = DIR_FORWARD;  }
            else if (current_phase == PHASE_B) { error = true;                            }
            // PHASE_D -> PHASE_D: keine Aenderung 
            break;

        default:
            error = true;
            break;
    }

    // Letzte Phase fuer naechsten Durchlauf speichern 
    last_phase = current_phase;
}
    
    */


}

