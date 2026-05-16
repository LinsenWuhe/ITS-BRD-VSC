#include "zeitmessung.h"

 //Berechnet Zeitspanne zwischen zwei Timestamps in Sekunden
double timer_get_duration(uint32_t t1, uint32_t t2)
{
    uint32_t diff = t2 - t1;  //differenz berechnen (funktioniert auch bei Überlauf -> rechnet nach differenz)


    //Umrechnung in Sekunden (1. Ticks in Mikrosekunden umrechnen (90 ticks die mikrosekunde) 2. in Sekunden umrechnen -> ohne double cast werden zahlen abgeschnitten)
    // TICKS_PER_US in timer.h datei = 90
    return (double)diff / (double)TICKS_PER_US / 1000000.0;
}