#include "GPIO_read.h"
#include "stm32f429xx.h"


//speichern den aktuellen Status der Kanäle
int kanal1;
int kanal2;



//überlegung, ob man kanal einfach mit isr einliest - wenn es nicht klappt, in main wieder zu status_drehscheibe_aufgabe2() wechseln, dort 2x geändert
//oder liest man ein, indem interrupt selbst bei anderem kanal schaut?
void status_drehscheibe_isr()
{
    
}

int status_drehscheibe_aufgabe2()
{

    kanal1 = GPIOF->IDR & (1 << 0);
	kanal2 = GPIOF->IDR & (1 << 1);

    return OK;
}


//Aus Aufgabe 2
// int s6_lesen(void)
// {
//     //1 um 6 nach links shiften und schauen, ob true rauskommt
//     if (GPIOF->IDR & (1 << 6)) 
//     {
//         return s6_gedrueckt;  //Wenn Taste gedrückt ist (1)
//     }
//     return s6_nicht_gedrueckt; // 0
// }


