#include "lcd.h"
#include "stm32f429xx.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>

extern volatile bool s6;
extern volatile bool phase_aux0;
extern volatile bool phase_aus1;


//muss man so nennen, damit CPU interruptfunktion findet
//für button s6
void EXTI9_5_IRQHandler(void)
{
    EXTI->PR = (1<<6);
    s6 = true;
}

//nach aufgabe verläuft in den beiden isrs die verwaltung der phasenwechsel des drehgebers und das erfassen des zeitstempels des auftretens eines phasenwechsels

// wird ausgeführt bei Flanke, also wenn kein Strom kommt (unsicher)
//für kanal 0
void EXTI0_IRQHandler(void)
{
    //zeitstempel holen
    uint32_t zeitstempel = getTimeStamp();

    EXTI->PR = (1<<0); // was genau macht das? Das einzige Peripheriegerät ist doch der pi pico, ist dies also hier notwendig?

    if (GPIOG->IDR & (1 << 0)) phase_aux0 = true;
    else phase_aux0 = false;
 
}


//für kanal 1
void EXTI1_IRQHandler(void)
{
    EXTI->PR = (1<<1);

    if (GPIOG->IDR & (1 << 1)) phase_aus1 = true;
    else phase_aus1 = false;
  
}

//vervollständigen
void get_drehgeberdaten(uint32_t *zeitstempel_out, uint32_t *zaehler_out) // wofür weden die Parameter gebraucht?
{

}

void get_drehgeberdaten2()
{

}


