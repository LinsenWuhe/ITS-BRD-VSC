/**
  ******************************************************************************
  * @file    main.c
  * @author  Ahmet / Jannik
  * @brief   Programm zur Kontrolle eines Drehreglers (simuliert mit Pi Pico)
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
#include "calc.h"
#include "LED_output.h"
#include "timer.h"
#include "util.h"
#include "GPIO_read.h"
#include <stdint.h>



int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	initTimer();
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	//lcdSetFont(24);
	textInit();

	extern char kanal1;
	extern char kanal2;
	uint32_t t_fenster_start = getTimeStamp();
	int32_t pulse_start = 0;
	bool berechnet = false;

//FSM-Loop
while(1) {
		
	//einlesen
	status_drehscheibe();
	
	//verarbeiten
	berechneAktuellePhase();
	berechnePhasenWechsel();

	//Zeitfenster prüfen
	uint32_t t_now = getTimeStamp();
	double t_diff = timer_get_duration(t_fenster_start, t_now);
	
	//Nach Aufgabenstellung -> Zeitfenster soll nach Phasenwechsel stattfinden
	if(t_diff >= 0.250 && !berechnet)
	{
		//phasenwechsel aufgetreten?
		if (gibPulseCount() != pulse_start) 
		{
			//zeitfenster schließen und berechnen
			berechneWinkel();
			berechneGeschwindigkeit(t_fenster_start, t_now, pulse_start);
			
			//neues Zeitfenster starten
			t_fenster_start = t_now;
			pulse_start = gibPulseCount();
			berechnet = true;

			statusDrucken();		
		}
	}

	//Nach 500 ms auf jeden Fall berechnen
    if (t_diff >= 0.500) {
        if (!berechnet) {
            // Noch nicht berechnet - jetzt auf jeden Fall 
            berechneWinkel();
            berechneGeschwindigkeit(t_fenster_start, t_now, pulse_start);
            statusDrucken();
        }
        // Neues Zeitfenster starten 
        t_fenster_start = t_now;
        pulse_start     = gibPulseCount();
        berechnet       = false;  // Reset für nächstes Fenster 
    }

	//ausgeben
	//statusDrucken();

}}