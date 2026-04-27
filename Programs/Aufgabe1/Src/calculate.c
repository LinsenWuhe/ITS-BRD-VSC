#include "calculate.h"
#include "stack.h"
#include "token.h"
#include <limits.h> //für int_min und int_max

int calculate_operation(char op) {
    int val1; //oberste Zahl vom Stack
    int val2; //untere Zahl vom Stack 
    int res; //zum Prüfen von Bereichsüberschreitungen - Speicher für Zwischenergebnis

    //Holt zwei Werte und schreibt sie in val1 und val2
    if (stack_pop(&val1) != STACK_OK) //Es gibt keine Zahl auf dem Stack
        return ERR_STACK_EMPTY;
    if (stack_pop(&val2) != STACK_OK) {
        stack_push(val1); // Den ersten Wert wieder zurücklegen
        return ERR_STACK_EMPTY; //es gibt nur eine Zahl auf dem Stack
    }

    switch (op) {
        case PLUS:
            if((val1 > 0) && (val2 > 0) && (val1 > INT_MAX - val2))               //Prüfen ob val1 positiv ist (bei positiv und negativ kann kein Überlauf kommen)dann Bereichsüberschreitung bei positiven Zahlen
                {
                    stack_push(val2);
                    stack_push(val1);
                    return ERR_OVERFLOW;                
                }
            if((val1 < 0) && (val2 < 0) && (val2 < INT_MIN - val1))                //Prüfen auf Bereichsüberschreitung bei negativen Zahlen
                {
                    stack_push(val2);
                    stack_push(val1);
                    return ERR_UNDERFLOW;
                }
            res = val2 + val1;
            break;

        case MINUS:
            //Überlauf-Check 
                if ((val2 > 0) && (val1 < 0) && (val2 > INT_MAX + val1)) 
                {
                    stack_push(val2);
                    stack_push(val1);
                    return ERR_OVERFLOW;
                }

                // Unterlauf-Check 
                if ((val2 < 0) && (val1 > 0) && (val2 < INT_MIN + val1)) 
                {
                    stack_push(val2);
                    stack_push(val1);
                    return ERR_UNDERFLOW;
                }
                res = val2 - val1;
                break;

        case MULT:  
            //0 dabei? Wegen Division später
            if (val1 == 0 || val2 == 0) 
            {
                res = 0;
            }
            else 
            {
                // Überprüfung auf Überlauf (Positives Ergebnis > INT_MAX)  Passiert bei + * + oder - * -
                if (((val1 > 0 && val2 > 0) || (val1 < 0 && val2 < 0))) 
                {
                    // ist |val2| > INT_MAX / |val1| 
                    if ((val1 > 0 && val2 > INT_MAX / val1) || (val1 < 0 && val2 < INT_MAX / val1))
                    {
                        stack_push(val2);
                        stack_push(val1);
                        return ERR_OVERFLOW;
                    }
                } 
                // Überprüfung auf Unterlauf (Negatives Ergebnis < INT_MIN) Passiert bei - * +
                else {
                    if (val1 > 0) 
                    { // val2 muss negativ sein
                        if (val2 < INT_MIN / val1) 
                        {
                            stack_push(val2);
                            stack_push(val1);
                            return ERR_UNDERFLOW;
                        }
                    }
                    else
                    { // val1 ist negativ, val2 ist positiv
                        if (val1 < INT_MIN / val2) 
                        {
                            stack_push(val2);
                            stack_push(val1);
                            return ERR_UNDERFLOW;
                        }
                    }
                }
                
                res = val2 * val1;
                }
                break;

        case DIV:
            //wenn durch 0 geteilt wird
            if (val1 == 0) 
            {
                stack_push(val2);
                stack_push(val1);
                return ERR_DIV_BY_ZERO;
            }
            // Der Überlauf bei: INT_MIN / -1
            if (val2 == INT_MIN && val1 == -1) 
            {
                stack_push(val2);
                stack_push(val1);
                return ERR_OVERFLOW;
            }
            res = val2 / val1;
            break;

        //falls unbekannter Fehler auftritt
        default: 
            stack_push(val2);
            stack_push(val1);
            return ERR_UNKNOWN;
    }

    return stack_push((res)); // Ergebnis speichern
}
