/**
 ******************************************************************************
 * @file    display.c
 * 
 * @date    25. Juli 2024
 * @brief   Template der display.c f�r die Laborklausur GS SoSe 2024
 *
 * @author  
 *
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_general.h"
#include "error.h"
#include "fonts.h"
#include "fontsFLASH.h"
#include "lcd.h"
#include "main.h"
#include "display.h"

#define DIGIT_X 17
#define DIGIT_Y 24
#define SPACER 8
#define OFFSET 6*17
#define BLOCK_X (SPACER+DIGIT_X*4)
#define BLOCK_Y (SPACER+DIGIT_Y*4)

piece_t pieces[9];

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

void initPuzzle(void) {
	// Initialisierung aller Puzzleteile mit der angezeigten Nummer 
	// darf nicht ver�ndert werden
	pieces[0].number = 1;
	pieces[1].number = 0;
	pieces[2].number = 2;
	pieces[3].number = 3;
	pieces[4].number = 4;
	pieces[5].number = 5;
	pieces[6].number = 6;
	pieces[7].number = 7;
	pieces[8].number = 8;

	// Initialisierung der Position auf dem Spielfeld
	for (int i=0; i<9; i++) {
		initPiecePosition(i);
	}
}

void initPiecePosition(int pos)
{
	int xTL, yTL, xBR, yBR;
	
	xTL = OFFSET + BLOCK_X*(pos % 3);
	yTL = SPACER + BLOCK_Y*(pos / 3);

	xBR = xTL + BLOCK_X - SPACER;
	yBR = yTL + BLOCK_Y - SPACER;

	pieces[pos].tl.x = xTL;
	pieces[pos].tl.y = yTL;
	pieces[pos].br.x = xBR;
	pieces[pos].br.y = yBR;
}


void drawPiece(int pos) 
{
	if (pieces[pos].number == 0) 
	{
		GUI_drawRectangle(pieces[pos].tl, pieces[pos].br, BLACK, true, 1);
		return;
	}
	
	GUI_drawRectangle(pieces[pos].tl, pieces[pos].br, WHITE, true, 1);

	int xTL = pieces[pos].tl.x + ((BLOCK_X-SPACER)/2) - (DIGIT_X /2);
	int yTL = pieces[pos].tl.y + ((BLOCK_Y-SPACER)/2) - (DIGIT_Y /2);

	Coordinate charTL = {xTL, yTL};

	char ch = '0'+pieces[pos].number; 

	GUI_disChar(charTL, ch, &Font24, WHITE, BLACK);
}


void drawPuzzle(void) 
{
	for (int i = 0; i<9; i++) 
	{
		drawPiece(i);
	}
}

// EOF