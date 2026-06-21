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
	initITSboard();    //Initialisiert zwar board, aber nicht für uns passend den output der gpio ports
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch

	initTimer(); //für timer-modul

	// Zuerst Bits für Pin 0 und 1 im GPIOG-MODER löschen, um in einen neuen Zustand zu bringen gleich - jeder pin belegt 2 Bits
    GPIOG->MODER &= ~((3 << 0) | (3 << 2));

    // Beide Pins auf 01 (Output) setzen
    GPIOG->MODER |= ((1 << 0) | (1 << 2));

    // pg0 - opendrain
    OpenDrainAn(); //damit leitung nur aktiv auf 0 gezogen wird, keine kurzschlussgefahr wie bei pushpull
    
    // pg1 aktiv auf 3,3 v setzen, um den Sensor parasitär zu versorgen - bleibt die ganze Ziet als Push-pull geschaltet
    GPIOG->BSRR = (1 << 1);

	uint8_t byte_LSB;	//zum lesen später
	uint8_t byte_MSB;	

	uint8_t rom_id[8];		//platz für 64 bit hardwareadresse des sensors später
	char textausgabe[60];	//zum ausgeben

	uint8_t gefundene_id[8]; //auch zum speichern der hardwareadresse
	char text[60]; //zum ausgeben

	while(1)
	{
		//================================
		//1. messung für alle sensoren gleichzeitig starten
		//================================
		if(reset() == OK) //es ist mind. ein sensor da
		{
			//nach reset wird erwaretet read Rom oder match rom oder skip rom
			sendeByte(skip_ROM); //alle adressen ignorieren - ab jetzt gleichzeitig auf master hören - nach Reset warten Sensoren auf Adresse
			sendeByte(convert_t); //startbefehl für Temperaturmessung, schickt aber keine Daten zurück - später mit read scratchpad

			//Strom liefern für Messung
			PushPullAn(); //Pin kann nun aktiv Strom ausgeben - auf pushpull modus setzen
			GPIOG->BSRR = (1<<0); //PG0 auf HIGH setzen -> strom über datenleitung - erst jetzt wird strom ausgegeben

			HAL_Delay(750); //Messung durchführen und aufladen - braucht Sensor nach Datenblatt, um messung durchzuführen (12 bit)
			
			OpenDrainAn(); //nach Messung Leitung wieder auf low ziehen
		}
		else
		{
			//wenn Reset fehlschlägt, bricht Master ab, und sagt dass kein sensor angeschlossen ist
			lcdGotoXY(1,2);
			lcdPrintS("Fehler: Kein Sensor angeschlossen!");
			HAL_Delay(1000);
			continue; //springt an Anfang der Schleife zurück
		}

		//===============================
		//2. Schritt - Suchen
		//==============================

		//um id von sensoren zu lesen und rauszufinden, ob wir in while nochmal lesen müssen, weil wir eine andere Abwzweigung im Baum nehmen müssen
		//falls im Betrieb Sensoren dazukommen / entfernt werden, wird die die Suche trotzdem vor jedem Durchlauf mit starteSucheNeu auf 0 gesetzt, um Bus abzusuchen
		starteSucheNeu();
		int sensor_index = 0;

		//===============================
		//3. Schritt - Bus absuchen
		//==============================

		//es wird so lange nach dem nächsten Sensor gesucht und dieser verarbeitet, bis die Funktion sagt, dass der Bus komplett leer ist
		//sucheNaechstenSensor gibt eine 0 zurück bei Fertig/Fehler und eine 1 bei Erfolg
		while (sucheNaechstenSensor(gefundene_id)) 
		{
			sensor_index++; //zähler für displayzeile

			//1.CRC prüfen
			if(berechneCRC(gefundene_id, 8) != 0)
			{
				//sprintf schickt Text nicht an Anzeige, sondern schreibt  ihn in ein Array aus zeichen, hier damit wir String zusammenbauen können, geht in lcdPrintS nicht
				sprintf(text, "Sensor %d,: CRC Fehler!", sensor_index);
				lcdGotoXY(1, 1+ sensor_index);
				lcdPrintS(text);
				continue;
			}

			//2. Familiy Code prüfen
			if(gefundene_id[0] != 0x28)
			{
				sprintf(text, "Sensor %d: Falscher Typ!", sensor_index);
				lcdGotoXY(1, 1+sensor_index);
				lcdPrintS(text);
				continue;
			}

			//wenn ids valide sind -> daten von genau diesem sensor holen
			reset();
			sendeByte(match_ROM); 
			//zwischengespeicherte id wird geschickt
			for (int i = 0; i < 8; i++)
			{
				sendeByte(gefundene_id[i]);
			}
			sendeByte(read_Scratchpad);
	
			//gibt 2 8bit Werte aus
			byte_LSB = liesByte();
			byte_MSB = liesByte();
	
			//beide 8-bit werte zu einem 16 bit wert vereinen
			//msb verschieben und mit oder verknüpfen
			int16_t temperatur_roh = (byte_MSB << 8) | byte_LSB;
	
			//rohwert durch 16 teilen laut datenblatt, um °C zu erhalten
			float temperatur_celsius = temperatur_roh / 16.0f; //als float, um nachkommazahlen nicht abzuschneiden

			// Ausgabe auf dem LCD (Zeile 2, 3, 4, 5...)
            // Wir geben alle 8 Bytes (rom_id[0] bis [7]) als Hexadezimalwerte aus.
            // "%.1f" zeigt die Temperatur mit einer Nachkommastelle an und %02X -> Zahl als Hexadeziamlzahl formatiert mit 2 Stellen darstellen
            sprintf(text, "Sensor %d ID:%02X%02X%02X%02X%02X%02X%02X%02X - %.1f Grad", 
                    sensor_index, 
                    gefundene_id[0], gefundene_id[1], gefundene_id[2], gefundene_id[3],
                    gefundene_id[4], gefundene_id[5], gefundene_id[6], gefundene_id[7], 
                    temperatur_celsius);
                    
            // Jedes Mal, wenn ein neuer Sensor gefunden wird, springen wir eine Zeile tiefer
            lcdGotoXY(1, 1 + sensor_index);
            lcdPrintS(text);
		}
		// Löscht eine unverwendete Zeile
		lcdGotoXY(1, 1 + sensor_index + 1);
        lcdPrintS(" ");

		//wenn mit while schleife durch, gehen wir in übergeordnete und fangen wider von vorne an -> wieder zu starteSucheNEu
		HAL_Delay(1000);
	}
}

// EOF
