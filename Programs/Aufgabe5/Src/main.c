/**
  ******************************************************************************
  * @file    main.c
  * @author  Ahmet, jannik
  * @brief   Aufgabe II - Drehmotor GSP Praktikum
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "ISR_init.h"
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
#include "zeitmessung.h"
#include <stdint.h>
#include <stdio.h>

	//sollte überall sichtbar sein und wird immer aus ram geladen - innerhalb von main nur lokale Variablen
	volatile bool s6 = false;
	volatile int counter = 0;   //für Test vom Interrupt


int main(void) {
	initITSboard();  					  							 // Initialisierung des ITS Boards
	initTimer();
	GUI_init(DEFAULT_BRIGHTNESS);							 // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                  	  							// Initialisierung des LCD Boards mit Touch
	
	textInit();
	status_drehscheibe(); // nötig vor calc init für letztePhase
	calcInit();
	s6_isr_init(); //interrupt initialisieren
	
	extern int phasenzahl;											// Anzahl der Phasenwechsel
	// int letztePhasenzahl = 0;
	// int s6;

	// int phasenzahl = 0;	// Anzahl der Phasenwechsel
	// double winkel = 0; // Winkel
	// int bewegungsrichtung; // Bewegungsrichtung

	uint32_t t_fenster_start = getTimeStamp(); //Timer zählt nur vorwärts, keine negativen zahlen
	int32_t pulse_start = 0; //wie viele phasenwechsel passieren im zeitfenster
	bool berechnet = false;

	//PG2 für Textmessung
	__HAL_RCC_GPIOG_CLK_ENABLE(); //Clock aktivieren auf Port
	GPIOG->MODER |= (1 << 4); 	//Bit 4+5 auf Output setzen -> 01
	GPIOG->MODER &= ~(1 << 5);


	//PG3 als Output für mainloop-Messung
	GPIOG->MODER |= (1 << 6);
	GPIOG->MODER &= ~(1 << 7);


	
	//Superloop mit Direct Digital Control (einlesen, verarbeiten, ausgeben - DDC)
	while(1) 
	{
		GPIOG->ODR |= (1 << 3); //Pin PG3 auf High

		/*-----1. Einlesen--------*/
		uint32_t t_jetzt = getTimeStamp();
		status_drehscheibe(); 
		
		/**
		Hier nur die Infos aus ISR holen, bzw. Flag lesen - damit ISR schlank gehalten wird
		*/
		if(s6) 
		{
			//fehlerLoeschen();
			//statusDrucken();
			s6 = false;
			lcdPrintInt(counter);
			continue;												 //nächsten Loop starten und nicht mehr
		}
	
		
		/*-----2. Verarbeiten--------*/
		
		berechneAktuellePhase();
		//berechnePhasenwechsel(int aktuellePhase, int letztePhase, int *ergebnis);     Welche Parameter hier rein?? Lieber direkt alles in calc speichern?
		berechnePhasenwechsel2();
		
		//Bevor status gedruckt wurd -> kann hängen, so zeigen LEDs immer aktuellen status
		
		
		double t_differenz = timer_get_duration(t_fenster_start, t_jetzt);
		
		
		if (t_differenz >= 0.250 && !berechnet) //Zeitfenster soll nach Phasenwechsel stattfinden -> mindestens 250ms vergangen
		{
			//phasenwechsel aufgetreten?
			if (gibRichtung() != GLEICH)//gibPulseCount() != pulse_start) 
			{
				//zeitfenster schließen und berechnen
				berechneWinkel();
				berechneGeschwindigkeit(t_fenster_start, t_jetzt, pulse_start);
				
				//neues Zeitfenster starten
				t_fenster_start = t_jetzt;
				pulse_start = gibPulseCount();
				berechnet = true;


				//Ausgeben I
				statusDrucken();		
				t_fenster_start = t_jetzt; //neues zeitfenster starten
			}
		}
		

		if (t_differenz >= 0.500) //spätestens nach 500ms wird berechnet
		{
			if (!berechnet) //wenn noch nicht berechnet wurde
			{
			    // Noch nicht berechnet - jetzt auf jeden Fall 
            	berechneWinkel();
           		berechneGeschwindigkeit(t_fenster_start, t_jetzt, pulse_start);
				
				//Ausgeben II
            	statusDrucken();
			}
			t_fenster_start = t_jetzt; //neues zeitfenster starten
		}
		pulse_start = gibPulseCount(); //anfangsphasenzahl = aktuelle phasenzahl
		berechnet = false;	//nächstes Ergebnis wurde noch nicht berechnet
		updateLEDAusgabe(gibRichtung(), gibPulseCount()); // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs
		
		//PG3 auf low
		GPIOG->ODR &= ~(1 << 3);

		//printf("%f\n", t_differenz);

	 }
}

// EOF
