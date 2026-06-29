#ifndef __LED_OUTPUT_H
#define __LED_OUTPUT_H

#include "stm32f429xx.h"
#include "calc.h"
#include "error.h"

#define fehlerLED (0x1U << 5)

int updateLEDAusgabe(int bewegung, int phasenzahl);

int resetBewegungLEDAusgabe();

void fehlerLEDaus(void);

#endif