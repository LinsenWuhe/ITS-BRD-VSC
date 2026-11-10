/**
 ******************************************************************************
 * @file    display.c
 * @date    25. Juli 2024
 * @brief   Template der display.c fuer die Laborklausur GS SoSe 2024
 * @author  [Dein Name] [Dein Matrikelnummer]
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_general.h"
#include "game.h"
#include "main.h"
#include <stdlib.h>
#include "display.h"

#define DIGIT_X 17
#define DIGIT_Y 24
#define SPACER 8
#define OFFSET (6*17)
#define BLOCK_X (SPACER+DIGIT_X*4)
#define BLOCK_Y (SPACER+DIGIT_Y*4)

piece_t pieces[BLOCK_COUNT];
bool zahlenArray[9];

void initDisplay() {
    GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
    TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
    GUI_clear(BLUE);
    lcdSetFont(24);
    if (!checkVersionFlashFonts()) {
        // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
        Error_Handler();
    }
}

int gibZufallsZahlvon1bis9()
{
    return rand() %9;
}

void setzeArrayZurueck(bool* array)
{
    for (int i=0; i<9; i++) 
    {
        array[i] = false;
    }
}

void initPuzzle(void) {
    // Initialisierung aller Puzzleteile mit der angezeigten Nummer 
    // darf nicht veraendert werden


    if(gibZufallsReihenfolge())
    {    
        setzeArrayZurueck(zahlenArray);
        
        for(int i = 0; i<9; i++)
        {
            int position = 0;

            do {
            position = gibZufallsZahlvon1bis9();
            }
            while (zahlenArray[position] != false);
        
            pieces[i].number = position;

            zahlenArray[position] = true;
        }
    }
    else 
    {
        pieces[0].number = 1;
        pieces[1].number = 0;
        pieces[2].number = 2;
        pieces[3].number = 3;
        pieces[4].number = 4;
        pieces[5].number = 5;
        pieces[6].number = 6;
        pieces[7].number = 7;
        pieces[8].number = 8;
    }

    // Initialisierung der Position auf dem Spielfeld
    for (int i = 0; i < BLOCK_COUNT; i++) {
        initPiecePosition(i);
    }
}

/**
 * @brief Berechnet die Koordinaten fuer einen Block an Position pos.
 * Die Positionen gehen von 0 bis 8 (3x3 Raster).
 * @param pos Index des Blocks (0 bis 8)
 */
void initPiecePosition(int pos) {
    // Sicherheitsabfrage: pos out of range [cite: 286]
    if (pos < 0 || BLOCK_COUNT <= pos) {
        return;
    }

	piece_t temp = pieces[pos];

	int xpos = pos % 3;
	int ypos = pos / 3;
	
	int xTL = (OFFSET + (xpos *BLOCK_X));
	int yTL = SPACER + (ypos * BLOCK_Y);

	int xBR = (xTL + BLOCK_X - SPACER);
	int yBR = (yTL + BLOCK_Y - SPACER);

	Coordinate tempTL = {xTL, yTL};
	temp.tl = tempTL;

	Coordinate tempBR = {xBR, yBR};
	temp.br = tempBR;

	pieces[pos] = temp;
}

/**
 * @brief Zeichnet einen einzelnen Block (Rechteck + Zahl) auf das Display.
 * @param pos Index des zu zeichnenden Blocks
 */
void drawPiece(int pos) {
    // Sicherheitsabfrage: pos out of range [cite: 289]
    if (pos < 0 || BLOCK_COUNT <= pos) {
        return;
    }

	piece_t temp = pieces[pos];
	

	if(temp.number == 0)
	{
		GUI_drawRectangle((temp.tl), (temp.br), BLACK, true, 3);
		return;
	}

	Coordinate testtl = temp.tl;
	Coordinate testbr = temp.br;
	GUI_drawRectangle(testtl, testbr, WHITE, true, 3);

	int charX = (temp.tl.x + ((BLOCK_X-SPACER)/2) - (DIGIT_X/2));
	int charY = (temp.tl.y + ((BLOCK_Y-SPACER)/2) -(DIGIT_Y/2));
	Coordinate charTL = {charX, charY}; 
	char drucken = '0' + temp.number;

	GUI_disChar(charTL, drucken, &Font24, WHITE, BLACK);



}

/**
 * @brief Zeichnet das gesamte Spielfeld (alle 9 Bloecke).
 */
void drawPuzzle(void) 
{
	for(int i=0; i<9; i++)
	{
		drawPiece(i);
	}
}

// EOF