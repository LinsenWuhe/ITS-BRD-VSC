#include "errorhandling.h"
#include "display.h"

void gibStatus(int status){

switch (status) {

    case -1: 
        printStdout("Division durch Null \n");
        break;
    case -2: 
        printStdout("Stack ist leer \n");
        break;
    case -3: 
        printStdout("Zahlenbereich ueberschritten \n");
        break;
    case 1: 
        printStdout("Stack voll! \n");
        break;
    case -4: 
        printStdout("Unbekannter Fehler \n");
        break;
    case -11:
        printStdout("Nur eine Zahl auf dem Stack \n");
    case -5:
        printStdout("Zahlenbereich unterschritten");
    default:
        //printStdout("Erfolg!\n");
        break;
    

}
}