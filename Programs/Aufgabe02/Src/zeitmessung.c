#include "zeitmessung.h"

int initZeitmessung()
{
    initTimer();

    return 0;
}

int setzteZeitpunk()
{
    anfangsZeit = getTimeStamp();
    return 0;
}

// TODO: in ms umwandeln
int differenzZumZeitpunktInms(int* differenz)
{
    *differenz = getTimeStamp() - anfangsZeit;
    return 0;
}