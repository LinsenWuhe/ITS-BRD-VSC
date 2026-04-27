/**
  ******************************************************************************
  * @file    main.c
  * @author  Jannik Goseberg
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"

//eigene Importe
#include "scanner.h"
#include "token.h"
#include "display.h"
#include "stack.h"
#include "util.h"
#include "calculate.h"
#include "errorhandling.h"


//---------------------------------------------------------------------------------


int main(void) {

	//Initialisierungen
	initITSboard();    					 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
	initDisplay();						//Display anzeigen lassen
	TP_Init(false);                		// Initialisierung des LCD Boards mit Touch

    T_token currentToken;               //Deklaration für Eingabetoken
    int status;

   // char buffer[12];                    //temporäres Char-Array, um Zahlen in Ziffern umzuwandeln und auszugeben
    //int tempVal;                        //temporäre Variable für Berechnung

while(1) {
    
         //Auf Eingabe warten -> currentToken wird durch nextToken zusammengestellt
        currentToken = nextToken();

        //Welche Taste wurde gedrückt?
        switch(currentToken.tok) {
            
            //ist currentToken eine Nummer??
            case NUMBER:
                status = stack_push(currentToken.val);
                break;

            //wenn currentToken eine Rechenoperationen ist, wird der Typ genutzt (.tok)
            case PLUS: case MINUS: case MULT: case DIV:
                status = calculate_operation(currentToken.tok); //berechnet Werte und gibt int zurück, die in "Status" landet
                break;

            case PRT: // Taste 'p' - Zeige oberste Zahl
                status = stack_peek();
                break;

            case PRT_ALL: //Taste 'P' - Gesamten Stack drucken
                status = stack_print_all();
                break;

            case CLEAR: // Taste 'C' - Alles löschen
                status = stack_clear();
                break;

            case DOUBLE: // Taste 'd' - Verdopple obersten Wert
                status = stack_double();
                break;

            //Taste 'r' - vertauscht oberste zwei Werte
            case SWAP: 
                status = stack_swap();
                break;

            case OVERFLOW: // Scanner hat Zahl erkannt, die zu groß ist
                printStdout("Fehler: Zahl zu gross!\n");
                status = 0;
                break;

            case UNEXPECTED:
                printStdout("Unbekannte Taste!\n");
                status = 0;
                break;
                
            default:
                // Falls ENTER oder andere Zeichen kommen, einfach ignorieren
                break;
        }

        gibStatus(status);
    }
}
// EOF
