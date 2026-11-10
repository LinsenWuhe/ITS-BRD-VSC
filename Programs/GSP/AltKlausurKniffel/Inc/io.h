#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdint.h>

/**
* Schaltet die Anzahl der angegebenen LEDs von rechts nach links an
* @param noLeds die Anzahl der zu leuchtenden LEDs
*/
void setLEDs(uint8_t noLeds);

/**
 * Liefert die Info darüber, ob der angegebene Button gedrückt ist
 * @param button der abzufragende button, muss zwischen 0 und 7 liegen
 * @return Antwort auf Frage, ob Button gedrückt ist
 */
bool buttonPressed(uint8_t button);


#endif