/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf, modifiziert durch andere
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "input.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "BMP_types.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"

#define IDR_MASK_PIN_6 (0x01U << 6)
#define PIXELSIZE 16
#define PIXELCOUNT 100


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	
	initInput();
	GUI_clear(255);
	
	char gpiofPin6Pressed;

	char buffer[PIXELCOUNT];
	
	while(1) {
		gpiofPin6Pressed = (IDR_MASK_PIN_6 != (GPIOF->IDR & IDR_MASK_PIN_6));
		
		if (gpiofPin6Pressed)
		{
			openNextFile();
			COMread(buffer, PIXELSIZE, PIXELCOUNT);
		}

	}
}

// EOF
