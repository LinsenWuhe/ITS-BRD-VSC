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
	initITSboard();  					  							 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);							 // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                  	  							// Initialisierung des LCD Boards mit Touch
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	
	textInit();
	calcInit();

	extern int phasenzahl;											// Anzahl der Phasenwechsel
	int letztePhasenzahl = 0;
	int Bewegungsrichtung = UNBEKANNT; 								// Bewegungsrichtung

	//int phasenzahl = 0;	// Anzahl der Phasenwechsel
	double winkel = 0; // Winkel
	int bewegungsrichtung; // Bewegungsrichtung
	
	//Superloop mit Direct Digital Control (einlesen, verarbeiten, ausgeben - DDC)
	while(1) 
	{
		//1. Einlesen
		status_drehscheibe(); //kanal1 & kanal2 auslesen und speichern -> Zugriff mit "extern int kanal1"
		s6_lesen();			  //lesen, ob S6 gedrückt ist, wird noch nicht gespeichert -> liefert 0 oder 1 zurück
		//2. Verarbeiten
		/*-----1. Einlesen--------*/
		status_drehscheibe(); 										//kanal1 & kanal2 auslesen und speichern -> Zugriff mit "extern int kanal1"
		if(s6_lesen() == HIGH) //bzw LOW wegen high active?
		{
			fehlerLoeschen();
			statusDrucken();
			continue;												 //nächsten Loop starten und nicht mehr
		}


		/*-----2. Verarbeiten--------*/
		//TODO - Zeitfenster öffnen
		berechneAktuellePhase();
		//berechnePhasenwechsel(int aktuellePhase, int letztePhase, int *ergebnis);     Welche Parameter hier rein?? Lieber direkt alles in calc speichern?

		// Phasenzahl bestimmen fehlt

		//berechneWinkel(phasenzahl, &winkel);
		

		//3. Ausgeben
		updateLEDAusgabe(bewegungsrichtung, phasenzahl); // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs
		//TODO - Zeitfenster schließen
		if(phasenzahl != letztePhasenzahl) //ist ein Phasenwechsel aufgetreten??
		{	
			berechneWinkel();
			//berechneGeschwindigkeit

			//für nächsten Loop
										// TODO - Zeitfenster öffnen für nächsten loop
			letztePhasenzahl = phasenzahl;

		}

		/*-----3. Ausgeben--------*/
		updateLEDAusgabe(Bewegungsrichtung, phasenzahl);	 // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs
		statusDrucken(); 												//Text ausgeben mit Zeitmessung
	}
}

// EOF
