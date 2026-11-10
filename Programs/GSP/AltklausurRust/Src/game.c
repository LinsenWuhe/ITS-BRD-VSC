#include "game.h"
#include "LCD_GUI.h"
#include "display.h"
#include "io.h"
#include "lcd.h"
#include "main.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "touch.h"

extern piece_t pieces[BLOCK_COUNT];
int gedrueckt;
bool leds[8];

bool erstesSpiel = true;
bool zufallsreihenfolge = false;

bool isNeighbour(int pos0, int pos1)
{
    switch (pos1) 
    {
        case 0:
            return (pos0 == 1 || pos0 ==3);
        case 1:
            return (pos0 == 0 || pos0 ==4 || pos0 == 2);
        case 2:
            return (pos0 == 1 || pos0 == 5);
        case 3:
            return (pos0 == 0 || pos0 == 4 || pos0 == 6);
        case 4:
            return (pos0 == 1 || pos0 == 3 || pos0 == 5 || pos0 == 7);
        case 5:
            return (pos0 == 2 || pos0 == 4 || pos0 == 8);
        case 6:
            return (pos0 == 7 || pos0 ==3);
        case 7:
            return (pos0 == 4 || pos0 == 6 || pos0 == 8);
        case 8:
            return (pos0 == 5 || pos0 ==7);
        default:
            return false;
    }
}

void getPosLED(bool* leds)
{
    for(int i=0; i<9; i++)
    {
        piece_t zuPruefen = pieces[i];

        if (i == (zuPruefen.number))
        {
            leds[i] = true;
        }
        else {
        leds[i] = false;
        }
    }
}

bool checkGameOver(void)
{
    for (int i = 0; i<9; i++) 
    {
        piece_t zuPruefen = pieces[i];

        if(i != zuPruefen.number) 
        {
            return false;
        }
    }
    return true;
}

int findEmptyBlock()
{
    for (int i = 0; i<9; i++) {
        if (pieces[i].number == 0) 
        {
            return i;
        }
    }

    return -1;
}
	
void updateLEDs()
{
    getPosLED(leds);
    setLEDs(leds);
}

bool gibZufallsReihenfolge()
{
    return zufallsreihenfolge;
}

void warteAufNeuesSpiel()
{
    	lcdGotoXY(1, 1);
        lcdPrintReplS("Neues Spiel");  
        lcdGotoXY(1, 3);
        lcdPrintReplS("Einfache Reihenfolge: S0");
        lcdGotoXY(1, 4);
        lcdPrintReplS("Zufallsreihenfolge:   S1");
        
        while (((GPIOF->IDR & 1) != 0) && ((GPIOF->IDR & (1<<1)) != 0 ))
        {
        }

        if((GPIOF->IDR & (1<<1)) == 0 ) 
        {
            zufallsreihenfolge = true;
            initDisplay();
            initPuzzle();
            drawPuzzle();
        }
        else 
        {
            zufallsreihenfolge = false;
            initDisplay();
            initPuzzle();
            drawPuzzle();
        }

        erstesSpiel = false;
}

void startGame(void)
{


    if ((GPIOF->IDR & 1) == 0 || erstesSpiel) 
    {
        GUI_clear(BLUE);
        warteAufNeuesSpiel();
    }

    gedrueckt = getTouchedBlock();
    int leererBlock = findEmptyBlock();

    if (isNeighbour(gedrueckt, leererBlock)) 
    {
        int temp = pieces[gedrueckt].number;

        pieces[gedrueckt].number = pieces[leererBlock].number;

        pieces[leererBlock].number = temp;
        drawPuzzle();
    }

    updateLEDs();

    if (checkGameOver()) 
    {
        GUI_clear(BLUE);
        lcdGotoXY(7, 7);
        lcdPrintReplS("Gewonnen!!!!");
        HAL_Delay(3000);
        GUI_clear(BLUE);
        warteAufNeuesSpiel();
    }

}
	