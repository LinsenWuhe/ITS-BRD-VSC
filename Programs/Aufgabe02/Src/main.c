/**
  ******************************************************************************
  * @file    main.c
  * @author  Ahmet, jannik
  * @brief   Aufgabe II - Drehmotor GSP Praktikum
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "LED_output.h"
#include "calc.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include "GPIO_read.h"
#include "text_output.h"


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	
	textInit();
	calcInit();

	extern int phasenzahl;	// Anzahl der Phasenwechsel
	int Bewegungsrichtung = UNBEKANNT; // Bewegungsrichtung

	//Superloop mit Direct Digital Control (einlesen, verarbeiten, ausgeben - DDC)
	while(1) 
	{
		//1. Einlesen
		status_drehscheibe(); //kanal1 & kanal2 auslesen und speichern -> Zugriff mit "extern int kanal1"
		if(s6_lesen() == HIGH)
		{
			fehlerLoeschen();
			statusDrucken();
			continue; //nächsten Loop starten und nicht mehr
		}


		//2. Verarbeiten


		//3. Ausgeben
		updateLEDAusgabe(Bewegungsrichtung, phasenzahl); // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs
		statusDrucken(); //Text ausgeben mit Zeitmessung
	}
}

// EOF
