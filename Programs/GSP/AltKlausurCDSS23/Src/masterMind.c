#include "masterMind.h"
#include "LCD_GUI.h"
#include "display.h"
#include "random.h"
#include "terminal.h"
#include <locale.h>
#include <stdio.h>
#include <string.h>

extern Terminal terminal;

void printNumber(char *s, int size, uint8_t *sno, uint8_t *guess)
{
    char buffer[100];
    
    if (guess == NULL) 
    {
        sprintf(buffer, "%s%c", s, '\0');
        printStr(buffer, BLACK);

        for (int i = 0; i<size; i++) 
        {
            printChar(sno[i] + '0', &terminal);    
        }

        printTerm("\n", &terminal);
    }
    else 
    {

        sprintf(buffer, "%s%c", s, '\0');
        printStr(buffer, BLACK);

        for (int i = 0; i<size; i++) 
        {
            if (guess[i] == sno[i]) 
            {
                setTextColorTerm(BLUE, &terminal);
                printChar(guess[i] + '0', &terminal);
            }
            else 
            {
                setTextColorTerm(RED, &terminal);
                printChar(guess[i] + '0', &terminal);
            }
        }
    }

    setTextColorTerm(BLACK, &terminal);
    printTerm("\n", &terminal);
}

bool won(int size, uint8_t *sno, uint8_t *guess)
{
    for (int i = 0; i < size; i++) 
    {
        if (sno[i] != guess[i]) 
        {
            return false;
        }
    }

    return true;
}

void genNewNumber(int size, uint8_t *sno)
{

    for (int i = 0; i<size; i++) 
    {
        sno[i] = (uint8_t)(nxtRandNumber() % 10);
    }
}

// EOF