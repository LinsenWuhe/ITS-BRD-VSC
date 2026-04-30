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
#include "GPIO_read.h"



int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	extern char kanal1;
	extern char kanal2;

	lcdSetFont(24);

  // Begruessungstext	
	lcdPrintlnS("Drehscheibe");


//FSM-Loop
while(1) {
		
	//einlesen
	status_drehscheibe();


	//verarbeiten

	//ausgeben
	statusDrucken();

	HAL_Delay(500);
}}