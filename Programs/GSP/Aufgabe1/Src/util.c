#include "util.h"
#include "stack.h"
#include "display.h"
#include <limits.h>


char buffer[12];  //für Textausgaben


//Formale parameter mit int-Zahl und Adresse zu Char-Array (12 zeichen)
void intToString(int n, char *str) {

    //Sonderfall für INT_MIN
    if(n == INT_MIN){
        str[0] = '-';
        str[1] = '2'; str[2] = '1'; str[3] = '4'; str[4] = '7'; str[5] = '4'; 
        str[6] = '8'; str[7] = '3'; str[8] = '6'; str[9] = '4'; str[10] = '8';
        return;
    }

    int i = 0; //erster Index im array
    int isNegative = 0; //für negative Zahlen
    
    // Sonderfall: Die Zahl ist Null -> spätere while Schleife startet nach 0
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0'; //durch [ ] - Zugriff auf i-Positionen nach str Adresse
        return;
    }

    // Behandlung negativer Zahlen
    if (n < 0) {
        isNegative = 1; //merken
        n = -n;         //umwandeln in positive Zahl -> ist leichter für berechnung
    }

    // Ziffern von hinten nach vorne extrahieren und in Char umwandeln
    while (n != 0) {
        str[i++] = (char) (n % 10) + '0'; // Letzte Ziffer holen und in ASCII wandeln (+ASCII Wert von '0' -> damit nicht mit magic Numbers gerechnet wrid)
        n = n / 10;                // Letzte Ziffer abschneiden
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // String-Ende markieren -> Nullterminator

    //Ziffern liegen rückwärts im Array -> String umdrehen
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int stack_peek(void) {

    //Speichern vom Wert
    int val;

    //Prüfung, ob stack leer ist
    if(stack_pop(&val) != 0)
    {
        return STACK_EMPTY;
    }

    //wieder aufräumen
    stack_push(val);

    //Zahl ausgeben
    intToString(val, buffer);      
    printStdout("Oben: ");                 
    printStdout(buffer);
    printStdout("\n");
                
    return STACK_OK;
}

int stack_swap(void){

    int temp1;
    int temp2;

    //Prüfung ob Stack leer ist und schreibt ersten Wert in temp1
    if(stack_pop(&temp1) != 0){
        return STACK_EMPTY;
    }
    if(stack_pop(&temp2) != 0)
    {
        stack_push(temp1); //wieder aufräumen, wenn nur eine Zahl auf Stack ist
        return STACK_NUR1;
    }

    //Das eigentliche Tauschen
    stack_push(temp1);
    stack_push(temp2);

    //Ausgabe auf Display
    printStdout("Oberste Zahlen getauscht \n");


    return STACK_OK;
}

int stack_print_all(void){    

    int temp[STACK_SIZE];
    int tempVal;
    int zaehler = 0;

    while(stack_pop(&tempVal) != STACK_EMPTY)
    {
        //erster Teil
        printStdout("Position ");
        intToString(zaehler+1, buffer); //wegen Index/zaehler offset
        printStdout(buffer);
        printStdout(": ");

        //temporäres Array aufbauen
        temp[zaehler] = tempVal;
        zaehler++;

        //Zahl ausgeben
        intToString(tempVal, buffer);
        printStdout(buffer);
        printStdout("\n");
    }

    while(zaehler > 0)
    {
        //Stack zurückbauen
        zaehler--;
        tempVal = temp[zaehler];
         stack_push(tempVal);
    }

    printStdout("Ende des Stacks \n"); 

    return STACK_OK;
    
}

int stack_double(void){

    int temp;

    //ersten Wert holen und überprüfen, ob Stack leer ist
    if(stack_pop(&temp) != 0)
    {
        return STACK_EMPTY;
    }

    //Wert zurückschreiben
    stack_push(temp);

    //Wert zweites Mal pushen, wenn Platz ist
    if(stack_push(temp) != 0)
    {
        return STACK_OVERFLOW;
    }

    printStdout("Wert dupliziert! \n");

    return STACK_OK;
}
