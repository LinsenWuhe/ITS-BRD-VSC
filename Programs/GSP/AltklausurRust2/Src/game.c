/**
 ******************************************************************************
 * @file    game.c
 *
 * @date    25. Juli 2024
 * @brief   Template der game.c f�r die Laborklausur GS SoSe 2024
 *
 * @author  
 *
 ******************************************************************************
 */



#include "display.h"
#include "lcd.h"
#include "main.h"
#include "../Inc/touch.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "../Inc/io.h"

extern piece_t pieces[9];
bool leds[8];

bool isNeighbour(int pos0, int pos1) 
{
    switch (pos0) 
    {
        case 0:
            return ((pos1 == 1)||(pos1 == 3));
        case 1:
            return ((pos1 == 0)||(pos1 == 4) || (pos1 == 2));  
        case 2:
            return ((pos1 == 1)||(pos1 == 5));
        case 3:
            return ((pos1 == 0)||(pos1 == 4) || (pos1 == 6));  
        case 4:
            return ((pos1 == 1)||(pos1 == 5) || (pos1 == 3) || (pos1 == 7));  
        case 5:
            return ((pos1 == 2)||(pos1 == 4) || (pos1 == 8));  
        case 6:
            return ((pos1 == 3)||(pos1 == 7));
        case 7:
            return ((pos1 == 4)||(pos1 == 6) || (pos1 == 8));  
        case 8:
            return ((pos1 == 7)||(pos1 == 5));
        default:
            return false;
    }
}

void getPosLED(bool* leds) 
{
    for (int i = 1; i<9; i++) 
    {
        if (pieces[i].number == i) 
        {
            leds[i-1] = true;
        }
        else 
        {
            leds[i-1] = false;
        }
    }
}

bool checkGameOver(void) 
{
    for (int i = 1; i<9; i++) 
    {
        if (pieces[i].number != i) 
        {
           return false;
        }
    }

    return true;
}

void initLeds()
{
    for (int i = 0; i<8; i++) 
    {
        leds[i] = false;
    }
}

int findeSchwarzenBlock()
{
    for (int i = 0; i<9; i++) 
    {
        if (pieces[i].number==0) 
        {
            return i;
        }
    }
    return -1;
}

void startNewGame()
{
        initDisplay();
        initPuzzle();
        initLeds();
        drawPuzzle();
}

void startGame(void)
{
    int block1 = getTouchedBlock();
    int block2 = findeSchwarzenBlock();

    if (isNeighbour(block1, block2)) 
    {
        int temp = pieces[block1].number;
        pieces[block1].number = pieces[block2].number;
        pieces[block2].number = temp;
        drawPuzzle();
    }

    getPosLED(leds);
    setLEDs(leds);

    if (checkGameOver()) 
    {
        lcdGotoXY(0, 1);
        lcdPrintReplS("Gewonnen, Neustart mit S0");
        while ((GPIOF->IDR&1)!=0) {}
        startNewGame();
    }

    if((GPIOF->IDR & 1) == 0)
    {
        startNewGame();
    }
}

// EOF