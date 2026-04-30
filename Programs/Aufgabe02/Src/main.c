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
#include "util.h"



int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

  // Begruessungstext	
	lcdPrintlnS("Hallo liebes TI-Labor (c-project)");
	
//FSM als Superloop
	while(1) {

		//einlesen


		//verarbeiten 


		//ausgeben


	}
}
