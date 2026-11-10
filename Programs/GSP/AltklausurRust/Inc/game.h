#ifndef GAME_H
#define GAME_H

#include "main.h"

/**
* Prüft, ob der geklickte Block überhaupt ein Nachbar der Lücke ist
* @param pos0 Berührter Block
* @param pos1 Lücke
*/
bool isNeighbour(int pos0, int pos1);

void getPosLED(bool* leds);

bool checkGameOver(void);

int findEmptyBlock();
	
void updateLEDs();

void startGame();

bool gibZufallsReihenfolge();
	
#endif
