/**
 ******************************************************************************
 * @file    main.c
 * @author  TI Team
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    17. Juli 2020
 * @brief   Rahmen fuer C Programme, die auf dem ITSboard ausgefuehrt werden.
 *
 *					25.04.2022 Tobias De Gasperis: printf hinzugefuegt
 ******************************************************************************
 */
 
#include "display.h"
#include "io.h"
#include "keypad.h"
#include "masterMind.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include "terminal.h"
#include "test.h"

#define MAX_TRYS               16   // Maximale Anzahl an Versuchen
#define NO_DIGITS               4   // Anzahl Stellen der gesuchten Zahl

// Die gesuchte und die akt. geratene Zahl werden als ein Feld von ASCII Zeichen dargestellt.
static uint8_t sno[NO_DIGITS];         // Die gesuchte Zahl
static uint8_t guess[NO_DIGITS];       // Die akt. geratene Zahl

int anzahlVersuche;
extern Terminal terminal;

void starteNeu()
{
	anzahlVersuche = 0;
	initDisplay();
	genNewNumber(NO_DIGITS, sno);
	printStr("Bitte eine Zahl raten \n", BLACK);
}


int main(void){
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	
    //testIO();
   //testDisplay();
    //testMasterMind();

	starteNeu();
	
	// Super Loop gemaess DDC
	anfang:
	while (1)
	{ 

		//einlesen
		if (anzahlVersuche == 16) 
		{
			printStr("Leider verloren, \n * fuer neues Spiel", RED);
			char next = getKeyPadInput();
			while (next != RESET_AND_HELP_KEY) 
			{
				next = getKeyPadInput();
			}
			starteNeu();
			continue;
		}

		for (int i = 0; i<NO_DIGITS; i++) 
		{
			char next = getKeyPadInput();
			if(next >= '0' && next <= '9')
			{
				guess[i] = next - '0';
				printChar(next, &terminal);
			}
			if (next == RESET_KEY) 
			{
				goto anfang;
			}
		}
		printTerm("\n", &terminal);

		//verarbeiten
		anzahlVersuche++;
		if (won(NO_DIGITS, sno, guess)) 
		{
			printStr("Du hast gewonnen, * fuer neues SPiel!", BLACK);
			char next = getKeyPadInput();
			while (next != RESET_AND_HELP_KEY) 
			{
				next = getKeyPadInput();
			}
			starteNeu();
			continue;
		}
		printNumber("Geraten: ", NO_DIGITS, sno, guess);


		//ausgeben
		setLEDs(anzahlVersuche);


	}
}

// EOF
