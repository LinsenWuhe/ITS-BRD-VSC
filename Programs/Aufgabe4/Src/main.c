/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include "timer.h"
#include "../Inc/switchMode.h"
#include "../Inc/master.h"
#include <stdint.h>
#include <stdio.h>


int main(void) {
	initITSboard();    //ggf nochmal MODER prüfen
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer(); //für timer-modul

	// Zuerst Bits für Pin 0 und 1 im GPIOG-MODER löschen
    GPIOG->MODER &= ~((3 << 0) | (3 << 2));

    // Beide Pins auf 01 (Output) setzen
    GPIOG->MODER |= ((1 << 0) | (1 << 2));

    // pg0 - opendrain
    GPIOG->OTYPER |= (1 << 0);
    
    // pg1 aktiv auf 3,3 v setzen, um den Sensor parasitär zu versorgen
    GPIOG->BSRR = (1 << 1);

	uint8_t byte_LSB;	//zum lesen später
	uint8_t byte_MSB;


	//1x Sensoren einlesen Einlesen und speichern - hier und nicht in der while oder?
	
	while(1)
	{
		reset();

		//Befehl Skip ROM
		sendeByte(0xCC);

		//befehl convert t - startet die temperaturmessung - datenblatt seite 11
		sendeByte(0x44);

		HAL_Delay(750);

		//erneut reset, um eine neue KOmmunikaitonsphase zu starten - weil sensor mit 12 bit misst, müssen wir nochmal messen - passt nicht in ein byte
		//nochmal reset, um der reihenfolge zu folgen: Rest, Rom command, function command (protokoll 1 wire)
		reset();

		//Skip Rom
		sendeByte(0xCC);

		//Befehl read scratchpad
		sendeByte(0xBE);


		byte_LSB = liesByte();

		byte_MSB = liesByte();


		//beide 8-bit werte zu einem 16 bit wert vereinen
		//msb verschieben und mit oder verknüpfen
		int16_t temperatur_roh = (byte_MSB << 8) | byte_LSB;

		//rohwert durch 16 teilen laut datenblatt, um °C zu erhalten
		float temperatur_celsius = temperatur_roh / 16.0f;

		char textausgabe[50];

		sprintf(textausgabe, "Temperatur: %.1f C", temperatur_celsius);

		lcdGotoXY(1,2);

		lcdPrintS(textausgabe);

		HAL_Delay(1000);





	}
}

// EOF
