#include "phase.h"
#include "stm32f429xx.h"
#include "ledausgabe.h"


int updateLEDAusgabe(int bewegung, int phasenzahl)
{
    // TODO: Richtigkeit überprüfen, denn eventuell nicht richtig
		if ((bewegung != GLEICH) && (bewegung != FEHLER))
		{
			GPIOD->BSRR = 0xFF << 16;
			GPIOD->BSRR = phasenzahl;
			
			if (bewegung == VORWAERTS)
			{
				GPIOE->BSRR = (0x1U << (6 + 16)); //unischer // D22 aus
				GPIOE->BSRR = (0x1U << 7); // D23 an
			}
			else
			{
				GPIOE->BSRR = (0x1U << (7 + 16)); //unsicher // D23 aus
				GPIOE->BSRR = (0x1U << 6);
			}
		}
		else if (bewegung == FEHLER)
		{
			GPIOE->BSRR = (0x1U << (6 + 16)); //unischer // D22 aus
			GPIOE->BSRR = (0x1U << (7 + 16)); //unsicher // D23 aus

			GPIOE->BSRR = (0x1U << 5); // D21 an
		}
	
	return 0;
}

int resetBewegungLEDAusgabe()
{
	GPIOE->BSRR = (0x1U << (6 + 16)); //unischer // D22 aus
	GPIOE->BSRR = (0x1U << (7 + 16)); //unsicher // D23 aus
	GPIOE->BSRR = (0x1U << (5 + 16)); //unischer // D21 aus
}