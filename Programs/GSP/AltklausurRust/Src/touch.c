#include "LCD_Touch.h"
#include "LCD_general.h"
#include "main.h"
#include <stdio.h>

extern piece_t pieces[BLOCK_COUNT];


int getBlockPos(Coordinate c)
{
	int xWert = c.x;
	int yWert = c.y;

	for (int i=0; i<9; i++) 
	{
		piece_t zuVergleichen = pieces[i];
		Coordinate tl = zuVergleichen.tl;
		Coordinate br = zuVergleichen.br;

		//x-Wert und y-Wert prüfen
		if((xWert >= tl.x && xWert <= br.x) && (yWert >= tl.y && yWert <= br.y))
		{
			printf("Gedrückt wurde Block an Position %d \n", i);
			return i;
		}
	}

	printf("Außerhalb vom gültigen Bereich geklickt");
	return -1;
}


int getTouchedBlock(void)
{
	if(!TP_pressed())
	{
		return -1;
	}

	return getBlockPos(TP_get());
}