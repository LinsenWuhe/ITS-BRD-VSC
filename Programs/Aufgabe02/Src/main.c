/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf, modifiziert durch andere
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "LED_output.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include "GPIO_read.h"


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	
	
	int phasenzahl = 0;	// Anzahl der Phasenwechsel
	int Bewegungsrichtung; // Bewegungsrichtung

	//Superloop mit Direct Digital Control (einlesen, verarbeiten, ausgeben - DDC)
	while(1) 
	{
		//1. Einlesen
		status_drehscheibe(); //kanal1 & kanal2 auslesen und speichern -> Zugriff mit "extern int kanal1"
		s6_lesen();			  //lesen, ob S6 gedrückt ist, wird noch nicht gespeichert -> liefert 0 oder 1 zurück


		//2. Verarbeiten


		//3. Ausgeben
		updateLEDAusgabe(Bewegungsrichtung, phasenzahl); // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs

	}
}

// EOF
