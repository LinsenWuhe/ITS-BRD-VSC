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
#include "keypad.h"
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
	makeKeyPad(); 						// Keypad anzeigen lassen

    T_token currentToken;               //Deklaration für Eingabetoken
    int status;

    char buffer[12];                    //temporäres Char-Array, um Zahlen in Ziffern umzuwandeln und auszugeben
    int tempVal;                        //temporäre Variable für Berechnung

while(1) {
        //Auf Eingabe warten -> currentToken wird durch nextToken zusammengestellt
        currentToken = nextToken();

        //Welche Taste wurde gedrückt?
        switch(currentToken.tok) {
            
            //ist currentToken eine Nummer??
            case NUMBER:
                if (stack_push(currentToken.val) == STACK_OK) {
                    printStdout("Zahl gespeichert.\n"); //-> dann speichere den Wert (.val)
                } else {
                    printStdout("Fehler: Stack voll!\n");
                }
                break;

            //wenn currentToken eine Rechenoperationen ist, wird der Typ genutzt (.tok)
            case PLUS: case MINUS: case MULT: case DIV:
                {
                    status = calculate_operation(currentToken.tok); //berechnet Werte und gibt int zurück, die in "Status" landet

                    if(status == CALC_OK){
                        printStdout("Ergebnis berechnet \n");
                    }
                    else if (status == ERR_DIV_BY_ZERO){
                        printStdout("Fehler: Division durch 0\n");
                    }
                    else if (status == ERR_OVERFLOW) {
                        printStdout("Fehler: Zahlenbereich ueberschritten");
                    }
                    else if(status == ERR_STACK_EMPTY) {
                        printStdout("Fehler: Zu wenig Zahlen auf dem Stack \n");
                    }
                    else if(status == ERR_UNKNOWN){
                        printStdout("Unbekannter Fehler aufgetreten!");
                    }
                }
                break;

            case PRT: // Taste 'p' - Zeige oberste Zahl
                //Prüfung, ob Stack leer ist
                if (stack_peek(&tempVal) == STACK_OK) { //Adresse von "tempVal" wird übergeben (&) - Methode schreibt Wert an diese Adresse (*) oder liefert Fehlercode
                    intToString(tempVal, buffer);       //Wert wird an Methode übergeben - Zahlen aus TempVal werden als Zeichen in Buffer geschrieben
                    printStdout("Oben: ");                 // -> Funktionen aus display.h
                    printStdout(buffer);
                    printStdout("\n");
                } 
                //Wenn Stack leer ist
                else {
                    printStdout("Stack ist leer.\n");
                }
                break;

            case PRT_ALL: //Taste 'P' - Gesamten Stack drucken
            {
                int zaehler = 1;                         //für Stackpositionen
                setzeTopTemp(gibTop());                  //top holen und temp für Methode nutzen

                //Prüfung ob Stack leer ist
                if(gibTop() == -1)                         
                {
                    printStdout("Stack ist leer!\n");
                }      

                //ansonsten zahlen ausgeben, bis "topTemp" durch den Stack gelaufen ist
                while(gibTopTemp()>=0){
                    printStdout("Position ");                               //"Position:" ausgeben 
                    intToString(zaehler, buffer);
                    printStdout(buffer);
                    zaehler++;                                                   //Positionszaehler erhöhen
                    printStdout(": ");
                    intToString(gibZahlVonStack(gibTopTemp()), buffer);  //Zahl holen vom Stack und ausgeben, ohne zu verändern
                    printStdout(buffer);
                    printStdout("\n");
                    setzeTopTemp(gibTopTemp()-1);                               //"temp" um 1 verringern
                }
                break;
            }

            case CLEAR: // Taste 'C' - Alles löschen
                stack_clear();  //Stack löschen
                clearStdout();  //Bildschirm löschen
                printStdout("Stack geloescht.\n");
                break;

            case DOUBLE: // Taste 'd' - Verdopple obersten Wert
                if (stack_peek(&tempVal) == STACK_OK) {
                    stack_push(tempVal);
                    printStdout("Wert dupliziert.\n");
                }
                else printStdout("Stack ist leer\n");
                break;

            //Taste 'r' - vertauscht oberste zwei Werte
            case SWAP: 
                status = stack_swap();
                break;

            case OVERFLOW: // Scanner hat Zahl erkannt, die zu groß ist
                printStdout("Fehler: Zahl zu gross!\n");
                break;

            case UNEXPECTED:
                printStdout("Unbekannte Taste!\n");
                break;
                
            default:
                // Falls ENTER oder andere Zeichen kommen, einfach ignorieren
                break;
        }

        gibStatus(status);

    }
}
// EOF
