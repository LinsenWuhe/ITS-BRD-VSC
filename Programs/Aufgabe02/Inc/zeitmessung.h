#ifndef __ZEITMESSUNG_H
#define __ZEITMESSUNG_H

#include "timer.h"
#include <stdint.h>

int anfangsZeit; // oder uint32_t?

int initZeitmessung();

int setzteZeitpunk();

int differenzZumZeitpunkt(int* differenz);

#endif