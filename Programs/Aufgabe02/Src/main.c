/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf, modifiziert durch A
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "lcd.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "phase.h"


#define IDR_MASK_PIN_0	(0x01U << 0)
#define IDR_MASK_PIN_1 (0x01U << 1)

#define BSRR_MASK_PIN_2 (0x01U << 2)
#define BSRR_MASK_PIN_3 (0x01U << 3)

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch


	/*
		Sei PIN0 Kanal A und PIN1 Kanal B:
	*/

	char gpiofPin0Pressed, gpiofPin1Pressed;



	int aktuellePhase;
	int bewegung;
	int phasenzahl = 0;

	while(1) {
		gpiofPin0Pressed = (IDR_MASK_PIN_0 != (GPIOF->IDR & IDR_MASK_PIN_0));
		gpiofPin1Pressed = (IDR_MASK_PIN_1 != (GPIOF->IDR & IDR_MASK_PIN_1));

		phase(gpiofPin0Pressed, gpiofPin1Pressed, &aktuellePhase);
		phasenwechsel(aktuellePhase, &bewegung);

		if (bewegung == VORWAERTS) phasenzahl++;
		else if (bewegung == RUECKWAERTS) phasenzahl--;

		// TODO: Wie wird letztePhase am Anfang gesetzt?
		letztePhase = aktuellePhase;

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

		/*
		if (gpiofPin1Pressed)
		{
			GPIOD->BSRR = BSRR_MASK_PIN_2;
			GPIOE->BSRR = (0x1U << 17);
		}
		else
		{
			GPIOD->BSRR = BSRR_MASK_PIN_2 << 16;
		}
			*/
	}
}

// EOF
