#ifndef __LED_OUTPUT_H
#define __LED_OUTPUT_H

#include "stm32f429xx.h"
#include "calc.h"
#include "error.h"

int updateLEDAusgabe(int bewegung, int phasenzahl);

int resetBewegungLEDAusgabe();

#endif