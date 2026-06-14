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


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer(); //für timer-modul

	GPIOG->OTYPER |= (1<<PD1); //1x am Anfang, um PD1 auf 3,3V zu setzen

	//1x Sensoren einlesen Einlesen und speichern - hier und nicht in der while oder?
	
	while(1) {

		//Temperaturwerte lesen

		//temperaturwerte ausgeben
		

		HAL_Delay(10000);
	}
}

// EOF
