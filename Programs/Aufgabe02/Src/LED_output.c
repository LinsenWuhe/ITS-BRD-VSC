#include "LED_output.h"
#include "stm32f429xx.h"


//braucht als Parameter, welche LED eingeschaltet werden soll
void greune_LED_an(int LED){
//merkt sich, welche LED angeschaltet werden soll
int anschalten;

switch (LED) {
case 23: 
        anschalten = LED_vorwaertsDrehung;
        break;
case 22:
        anschalten = LED_rueckwaertsDrehung;
        break;
case 21:
        anschalten = LED_fehler;
        break;
default: 
        return;
}
    //schreibt an gewünschte LED Position ein 1 - alle anderen Ziffern bleiben gleich
    GPIOE->ODR |= anschalten;
}

void gruene_LED_aus(int LED){
    
int ausschalten;

switch (LED) {
case 23: 
        ausschalten = LED_vorwaertsDrehung;
        break;
case 22:
        ausschalten = LED_rueckwaertsDrehung;
        break;
case 21:
        ausschalten = LED_fehler;
        break;
default: 
        return;
}
    //negiert alle Bits -> schreibt dann an gewünschte Position eine 0, alle anderen Bits bleiben unverändert
    GPIOE->ODR &= ~ausschalten;
}

void blaue_LED_an(int LED){
    
int anschalten;

switch (LED) {
case 8: 
        anschalten = (1);
        break;
case 9:
        anschalten = (1 << 1);
        break;
case 10:
        anschalten = (1 << 2);
        break;
case 11: 
        anschalten = (1 << 3);
        break;
case 12:
        anschalten = (1 << 4);
        break;
case 13:
        anschalten = (1 << 5);
        break;
case 14: 
        anschalten = (1 << 6);
        break;
case 15:
        anschalten = (1 << 7);
        break;
default: 
        return;
}

    GPIOD->ODR |= anschalten;
}

void blaue_LED_aus(int LED){
    
int ausschalten;

switch (LED) {
case 8: 
        ausschalten = (1);
        break;
case 9:
        ausschalten = (1 << 1);
        break;
case 10:
        ausschalten = (1 << 2);
        break;
case 11: 
        ausschalten = (1 << 3);
        break;
case 12:
        ausschalten = (1 << 4);
        break;
case 13:
        ausschalten = (1 << 5);
        break;
case 14: 
        ausschalten = (1 << 6);
        break;
case 15:
        ausschalten = (1 << 7);
        break;
default: 
        return;
}

    GPIOD->ODR &= ~ausschalten;
}