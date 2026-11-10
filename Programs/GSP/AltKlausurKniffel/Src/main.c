/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Die Klausuraufgabe Kniffel.
 *
 ******************************************************************************
 */

#include "main.h"
#include "LCD_GUI.h"
#include "hilfsfunktionen.h"
#include "init.h"


int main(void){
   // Initialisierung der HW
   initITSboard();
	 GUI_init(DEFAULT_BRIGHTNESS);
 // testIO();
 //testDisplay();
// testKniffel();

   allesReset();

while (1) 
{
   //einlesen
   liesGPIOein();

   //verarbeiten
   if(kannNochGewuerfeltWerden())
   {
      verarbeiteGPIOs();
   
   //ausgeben
   pruefeGewinn();
   ledAusgabe();
   }
}
   
}
// EOF