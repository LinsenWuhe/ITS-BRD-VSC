#include "GPIO_read.h"
#include "stm32f429xx.h"


//speichern den aktuellen Status der Kanäle
int kanal1;
int kanal2;

int status_drehscheibe()
{
    // Auslesen von PF0 - Input Register lesen -> verundet mit 0000 0001
    if (GPIOF->IDR & (1 << 0)) 
    {
        // Pin PF0 ist HIGH -> Bedingung ist true
        kanal1 = HIGH;
    } 
    else 
    {
        // Pin PF0 ist LOW 
        kanal1 = LOW;
    }

    // Auslesen von PF1
    if (GPIOF->IDR & (1 << 1)) 
    {
        // Pin PF1 ist HIGH
        kanal2 = HIGH;
    }
    else
    {
        kanal2 = LOW;
    }

    //Absicherung -> beide Kanäle auch im Wertebereich
    if (kanal1 > 1 || kanal1 < 0 || kanal1 > 2 || kanal2 < 0) 
    {
        return kanal_fehler;
    }

    return OK;
}

int s6_lesen(void)
{
    //1 um 6 nach links shiften und schauen, ob true rauskommt
    if (GPIOF->IDR & (1 << 6)) 
    {
        return s6_gedrueckt;  //Wenn Taste gedrückt ist (1)
    }
    return s6_nicht_gedrueckt; // 0
}


