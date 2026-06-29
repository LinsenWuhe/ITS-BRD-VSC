#include "error.h"
#include "LED_output.h"
#include "calc.h"
#include "lcd.h"

extern bool fehlerBeiPhasenwechsel;


void fehlerLoeschen(void)
{
    calcInit();
    fehlerBeiPhasenwechsel = false;
    //fehlerLEDaus(); lieber:?
    resetBewegungLEDAusgabe(); 
    

    //Wenn Fehler gedruckt wird
    lcdGotoXY(6, 11);
    lcdPrintS("");
}
 