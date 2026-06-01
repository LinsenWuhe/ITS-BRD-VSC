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
	GUI_clear(BLACK);
	
	char gpiofPin6Pressed;
	static BITMAPFILEHEADER fh; //static, damit sie nicht auf stack landen, sondern im globalen Speicher liegen - erster Struct, der sagt, ob es überhaupt eine bmp datei ist, wie groß die datei ist und die pixeldatenanfangen
	static BITMAPINFOHEADER ih; //enthält alle Infos über das Bild, Breite, Höhe, Bits pro Pixel, Komprimierung und Anzahl der Palettenfarben
	static RGBQUAD palette[MAX_COLOR_TABLE_SIZE]; //Array mit bis zu 256 Einträgen. Jeder Eintrag ist ein RGBQUAD mit Felder Rot, Grün, Blau

	char buffer[PIXELCOUNT];
	
	while(1) {
		gpiofPin6Pressed = (IDR_MASK_PIN_6 != (GPIOF->IDR & IDR_MASK_PIN_6));
		
		if (gpiofPin6Pressed)
		{
			GUI_clear(BLACK);
			openNextFile();
	//bmp_reader aufrufen:
			BMP_readHeaders(&fh, &ih);
			BMP_readPalette(&ih, palette);
			BMP_decodeAndDisplay(&fh, &ih, palette);





			COMread(buffer, PIXELSIZE, PIXELCOUNT);
		}

	}
}

// EOF
