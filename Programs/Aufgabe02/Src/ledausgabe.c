#include "ledausgabe.h"
#include "phase.h"
#include "stm32f429xx.h"

int updateLEDAusgabe(int bewegung, int phasenzahl)
{
    // TODO: Richtigkeit überprüfen, denn eventuell nicht richtig
		if ((bewegung != GLEICH) && (bewegung != FEHLER))
		{
			GPIOD->BSRR = 0x00000000;
			GPIOD->BSRR = phasenzahl;

			if (bewegung == VORWAERTS)
			{
				GPIOE->BSRR = (0x1U << 15); //unischer
				GPIOE->BSRR = (0x1U << 7);
			}
			else
			{
				GPIOE->BSRR = (0x1U << 15); //unsicher
				GPIOE->BSRR = (0x1U << 6);
			}
		}
		else if (bewegung == FEHLER)
		{
			GPIOE->BSRR = (0x1U << 15); //unsicher
			GPIOE->BSRR = (0x1U << 15); //unsicher

			GPIOE->BSRR = (0x1U << 5);
		}
}