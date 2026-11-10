
#include "display.h"
#include "LCD_GUI.h"
#include "keypad.h"
#include "terminal.h"
#include <string.h>

Terminal terminal;

void initDisplay()
{
    terminal = makeKeyPad();
}

void printStr(char *s, uint16_t c)
{
    setTextColorTerm(c, &terminal);
    printTerm(s, &terminal);
}

void clearDisplay()
{
    clearTerm(&terminal);
}

char nextInput(bool resetOnly, int size, uint8_t *a)
{
    //wenn nur zurückgesetzt werden darf, wird so lange gewartet, bis reset gedrückt wurde
    if (resetOnly) 
    {
        char key;
        do 
        {
            key = getKeyPadInput();
        }
        while (key != RESET_KEY && key != RESET_AND_HELP_KEY);

        return key;
    } 

    int zaehler = 0;
    
    //liest so lange Zeichen ein, bis enter gedrückt wird
    while (1) 
    {
        char next = getKeyPadInput();

        if ((next == RESET_KEY)||(next == RESET_AND_HELP_KEY)) 
        {
            return next;
        }

        if (next == NEXT_GUESS_READ) 
        {
            return NEXT_GUESS_READ;
        }

        if (next >= '0' && next <= '9') 
        {
            if (zaehler < size) 
            {
                char str[3] = {next, '\n', '\0'};

                a[zaehler] = next - '0';
                zaehler++;
                printStr(str, RED);
            }
        }
    }
}

// EOF