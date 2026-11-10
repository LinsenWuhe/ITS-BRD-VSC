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
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "error.h"
#include "text_output.h"
#include "zeitmessung.h"
#include <stdint.h>

//sollte überall sichtbar sein und wird immer aus ram geladen - innerhalb von main nur lokale Variablen
volatile bool s6 = false;
//man kann den pico anschließen und signale geben -> wird oben im display angezeigt wie viele - erste 0 die s6 klicks, zweite 0 die impulse von kanal 1 oder 0 und dritte 0 der andere kanal

//variablen für isrs
volatile uint32_t phasenzaehler_isr = 0;
volatile uint32_t letzter_zeitstempel = 0;

//globale volatile Variablen für isrs
volatile Phase_t aktuellePhase;
volatile Phase_t letztePhase;
volatile int32_t phasenzahl = 0;
//volatile Richtung_t richtung = GLEICH; // Richtung ist hier nicht nötig, es ist ja in calc, muss auch nicht volatile sein, oder?
//volatile bool fehlerBeiPhasenwechsel = false; // gleiches wie bei richtung
volatile uint32_t letzterZeitstempel = 0;


int main(void) {
	initITSboard();  					  							 // Initialisierung des ITS Boards
	initTimer();
	GUI_init(DEFAULT_BRIGHTNESS);							 // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                  	  							// Initialisierung des LCD Boards mit Touch
	
	textInit();
	// V doch nicht nötig, weil calcInit getAktue... hat.
	//status_drehscheibe_aufgabe2(); //sollte hier richtig sein, sollte aber glaub ich angepasst werden. wie haben kanal1 und kanal2 und phase_aux...., das ist doch das gleiche?
	calcInit();
	s6_isr_init(); //interrupt initialisieren
	kanaele_isr_init();

	uint32_t t_fenster_start = getTimeStamp(); //Timer zählt nur vorwärts, keine negativen zahlen
	bool berechnet = false;

//Messung für Textausgabe Aufgabe 2
	// //PG2 für Textmessung
	// __HAL_RCC_GPIOG_CLK_ENABLE(); //Clock aktivieren auf Port
	// GPIOG->MODER |= (1 << 4); 	//Bit 4+5 auf Output setzen -> 01
	// GPIOG->MODER &= ~(1 << 5);
	// //PG3 als Output für mainloop-Messung
	// GPIOG->MODER |= (1 << 6);
	// GPIOG->MODER &= ~(1 << 7);


	
	//Superloop mit Direct Digital Control (einlesen, verarbeiten, ausgeben - DDC)
	while(1) 
	{	
		//Aufgabe 2 messung
		//GPIOG->ODR |= (1 << 3); //Pin PG3 auf High // kann einkommentiert werden

		/*-----1. Einlesen--------*/
		uint32_t t_jetzt = getTimeStamp();
		
		/**
		Hier nur die Infos aus ISR holen, bzw. Flag lesen - damit ISR schlank gehalten wird
		*/
		if(s6) 
		{
			fehlerLoeschen();
			statusDrucken();
			s6 = false;
			continue;												 //nächsten Loop starten und nicht mehr
		}
	
		
		/*-----2. Verarbeiten--------*/		
		double t_differenz = timer_get_duration(t_fenster_start, t_jetzt);
		
		
		if (t_differenz >= 0.250 && !berechnet) //Zeitfenster soll nach Phasenwechsel stattfinden -> mindestens 250ms vergangen
		{
			//phasenwechsel aufgetreten?
			if (gibRichtung() != GLEICH)
			{
				//zeitfenster schließen und berechnen
				berechneWinkel();
				berechneGeschwindigkeit(); //bekommt die Daten selbst durch get_drehgeberdaten
				
				//neues Zeitfenster starten
				t_fenster_start = t_jetzt;
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
				berechneGeschwindigkeit(); //bekommt die Daten selbst durch get_drehgeberdaten
				
				//Ausgeben II
            	statusDrucken();
			}
			t_fenster_start = t_jetzt; //neues zeitfenster starten
		}

		berechnet = false;	//nächstes Ergebnis wurde noch nicht berechnet
		updateLEDAusgabe(gibRichtung(), gibPulseCount()); // Ausgabe der Bewegungsrichtung/Fehler und Anzahl der Phasenwechsel auf den LEDs
		
		//PG3 auf low
		//GPIOG->ODR &= ~(1 << 3);

		//printf("%f\n", t_differenz);

	 }
}

// EOF
