#include "stack.h"
#include "display.h"

int data[STACK_SIZE]; // Stack für die Zahlen
int top = -1;         // Zeigt auf die oberste Zahl (-1 heißt leer)

int stack_push(int value) {

    // Prüfung, ob Stack schon voll ist
    if (top >= STACK_SIZE - 1) 
        return STACK_OVERFLOW; 

    //Auf Stack pushen und "Top" um 1 erhöhen (vor Zugriff)
    data[++top] = value;
    return STACK_OK;
}

int stack_pop(int *result) {

    //Prüfung ob Stack leer ist
    if (top < 0) 
        return STACK_EMPTY;

    //Wenn nicht leer -> pop 
    *result = data[top--];  //oberste Zahl wird an Adresse von "result" geschrieben und "top" dann einen verringert
    return STACK_OK;
}

int stack_clear(void) {
    //Zähler wird zurückgesetzt
    top = -1;

    //Ausgaben auf Display
    clearStdout();  //Bildschirm löschen
    printStdout("Stack geloescht.\n");

    return 0;
}
