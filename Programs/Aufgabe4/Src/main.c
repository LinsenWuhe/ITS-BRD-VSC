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
#include "../Inc/master.h"
#include <stdint.h>
#include <stdio.h>
#include "errors.h"


int main(void) {
	initITSboard();    //ggf nochmal MODER prüfen
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer(); //für timer-modul

	// Zuerst Bits für Pin 0 und 1 im GPIOG-MODER löschen
    GPIOG->MODER &= ~((3 << 0) | (3 << 2));

    // Beide Pins auf 01 (Output) setzen
    GPIOG->MODER |= ((1 << 0) | (1 << 2));

    // pg0 - opendrain
    GPIOG->OTYPER |= (1 << 0);
    
    // pg1 aktiv auf 3,3 v setzen, um den Sensor parasitär zu versorgen
    GPIOG->BSRR = (1 << 1);

	uint8_t byte_LSB;	//zum lesen später
	uint8_t byte_MSB;

	uint8_t rom_id[8];
	char textausgabe[60];


	//1x Sensoren einlesen Einlesen und speichern - hier und nicht in der while oder?
	
	while(1)
	{
		//================================
		//1. Ist ein Sensor angeschlossen??
		//================================
		if(reset() != OK)
		{
			//wenn Reset fehlschlägt, bricht Master ab, und sagt dass kein sensor angeschlossen ist
			lcdGotoXY(1,2);
			lcdPrintS("Fehler: Kein Sensor angeschlossen!");
			
			HAL_Delay(1000);
			continue; //springt an Anfang der Schleife zurück
		}

		//===============================
		//2. Schritt - 64 Bit Rom auslesen und CRC prüfen
		//==============================

		//befehl read ROM (0x33) laut Datenblatt
		sendeByte(0x33);

		for(int i = 0; i< 8; i++)
		{
			rom_id[i] = liesByte(); //alle 8 byte lesen
		}

		//Schlägt CRC prüfung fehl?
		if(berechneCRC(rom_id, 8) != 0)
		{
			lcdGotoXY(1, 2);
			lcdPrintS("Fehler: CRC ungueltig!");
			HAL_Delay(1000);
			continue; //durchlauf abbrechen - crc simmmt nicht
		}

		//stimmt familiy code?? (muss 0x28 sein bei dem sensor laut datenblatt)
		if (rom_id[0] != 0x28) 
		{
			lcdGotoXY(1, 2);
			lcdPrintS("Fehler: Ungueltiger Typ (familiy code)!");
			HAL_Delay(1000);
			continue; //durchlauf abbrechen - familiy code stimmt nicht
		}

		//wenn kein abbrauch (continue) kam, ist id geprüft
		//64-Bit ID formatiert ausgeben
		sprintf(textausgabe, "ID: %02X%02X%02X%02X%02X%02X%02X%02X",
			rom_id[0], rom_id[1], rom_id[2], rom_id[3], rom_id[4],
			rom_id[5], rom_id[6], rom_id[7]);
		lcdGotoXY(1, 2);
		lcdPrintS(textausgabe);

		//===========================================
		//3. Normale Temperaturmessung
		//==========================================
		reset();
		sendeByte(0xCC); //skip rom
		sendeByte(0x44); //convert t
		
        //PG0 kurz auf Push-Pull umschalten, damit aktiv 3,3V geliefert werden
        GPIOG->OTYPER &= ~(1 << 0); 
        GPIOG->BSRR = (1 << 0);     // Datenleitung aktiv auf HIGH

        HAL_Delay(750); // Der Sensor misst 

        // Nach den 750ms schalten wir PG0 sofort wieder auf Open-Drain zurück!
        GPIOG->OTYPER |= (1 << 0); 

		reset();
		sendeByte(0xCC); //Skip Rom
		sendeByte(0xBE); //Read Scratchpad


		byte_LSB = liesByte();
		byte_MSB = liesByte();

		//beide 8-bit werte zu einem 16 bit wert vereinen
		//msb verschieben und mit oder verknüpfen
		int16_t temperatur_roh = (byte_MSB << 8) | byte_LSB;

		//rohwert durch 16 teilen laut datenblatt, um °C zu erhalten
		float temperatur_celsius = temperatur_roh / 16.0f;

		//temperatur anzeigen
		sprintf(textausgabe, "Temperatur: %.1f C", temperatur_celsius);
		lcdGotoXY(1,3);
		lcdPrintS(textausgabe);

		HAL_Delay(1000);
	}
}

// EOF
