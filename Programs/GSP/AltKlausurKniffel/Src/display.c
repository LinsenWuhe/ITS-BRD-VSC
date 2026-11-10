/**
 ******************************************************************************
 * @file    display.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Dieses Modul implementiert die LCD Ausgabe der W�rfel.
 ******************************************************************************
 */

#include "display.h"
#include "LCD_general.h"
#include "main.h"
#include "LCD_GUI.h"
#include "error.h"
#include <stdbool.h>

// Dieses Feld speichert die Positionen der W�rfel. Die Position eines W�rfels
// ist durch seine linke obere und rechte untere Ecke beschrieben.

const struct {
	 Coordinate tl;
	 Coordinate br;
	} dicePosData[NO_DICE] = { {.tl = {.x = 382, .y = 40}, .br = {.x = 465, .y = 123}},
		                         {.tl = {.x = 292, .y = 40}, .br = {.x = 375, .y = 123}},
														 {.tl = {.x = 202, .y = 40}, .br = {.x = 285, .y = 123}},
														 {.tl = {.x = 112, .y = 40}, .br = {.x = 195, .y = 123}},
														 {.tl = {.x = 22,  .y = 40}, .br = {.x = 105, .y = 123}}};

// Von der linken oberen Ecke eines W�rfels ausgehend, beschreiben folgende Konstanten 
// den Offset der Mittelpunkte der einzelnen Positionen der W�rfelaugen / W�rfelpunkte.
//
//           |-------------------------|
//           |                         |
//           |  Pos1             Pos2  |
//           |                         |
//           |  Pos3    Pos4     Pos5  |
//           |                         |
//           |  Pos6             Pos7  |
//           |                         |
//           |-------------------------|
											
const Coordinate offsetPos1 = {.x = 1 * 21, .y = 1 * 21};
const Coordinate offsetPos2 = {.x = 3 * 21, .y = 1 * 21};
const Coordinate offsetPos3 = {.x = 1 * 21, .y = 2 * 21};
const Coordinate offsetPos4 = {.x = 2 * 21, .y = 2 * 21};
const Coordinate offsetPos5 = {.x = 3 * 21, .y = 2 * 21};
const Coordinate offsetPos6 = {.x = 1 * 21, .y = 3 * 21};
const Coordinate offsetPos7 = {.x = 3 * 21, .y = 3 * 21};

#define RADIUS_DICE_EYE       6   // 

extern int wuerfelWerte[5];

void clearDisplay()
{
	Coordinate tl = {0,0};
	Coordinate br = {479,319};

	GUI_drawRectangle(tl, br, BLUE, true,1 );
}

Coordinate addiereKoordinaten(Coordinate c1, Coordinate c2)
{
	Coordinate result;

	result.x = c1.x + c2.x;
	result.y = c1.y + c2.y;

	return result;
}

void displayDie(uint8_t die, uint8_t value, uint16_t c)
{
	GUI_drawRectangle(dicePosData[die].tl, dicePosData[die].br, c, false, 2);
	
	switch (value) 
	{
		case 1:
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos4)), RADIUS_DICE_EYE, c, true, 1);
			break;
		case 2:
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos6)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos2)), RADIUS_DICE_EYE, c, true, 1);
			break;
		case 3:
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos2)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos4)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos6)), RADIUS_DICE_EYE, c, true, 1);
			break;
		case 4: 
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos6)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos2)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos1)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos7)), RADIUS_DICE_EYE, c, true, 1);
			break;
		case 5:
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos1)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos7)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos4)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos6)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos2)), RADIUS_DICE_EYE, c, true, 1);
			break;
		case 6:
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos2)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos5)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos7)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos1)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos3)), RADIUS_DICE_EYE, c, true, 1);
			GUI_drawCircle((addiereKoordinaten(dicePosData[die].tl, offsetPos6)), RADIUS_DICE_EYE, c, true, 1);
			break;
		default:
			Error_Handler();
	}

	switch (die) 
	{
		case 0:
			wuerfelWerte[0] = value;
			break;
		case 1:
			wuerfelWerte[1] = value;
			break;
		case 2:
			wuerfelWerte[2] = value;
			break;
		case 3: 
			wuerfelWerte[3]= value;
			break;
		case 4: 
			wuerfelWerte[4] = value;
			break;
		default:
			Error_Handler();
	}
}


// EOF