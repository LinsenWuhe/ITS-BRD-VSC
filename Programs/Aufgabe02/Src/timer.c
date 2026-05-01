#include "timer.h"

 //Berechnet Zeitspanne zwischen zwei Timestamps in Sekunden
double timer_get_duration(uint32_t t1, uint32_t t2)
{
    uint32_t diff = t2 - t1;  /* Funktioniert auch bei Überlauf */

    /* TICKS_PER_US ist bereits in der ITS Lib definiert als 90 */
    return (double)diff / (double)TICKS_PER_US / 1000000.0;
}