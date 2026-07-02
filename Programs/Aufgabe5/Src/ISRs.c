#include "lcd.h"
#include "stm32f429xx.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>

extern volatile bool s6;


//muss man so nennen, damit CPU interruptfunktion findet
//für button s6
void EXTI9_5_IRQHandler(void)
{
    EXTI->PR = (1<<6);
    s6 = true;
}

//nach aufgabe verläuft in den beiden isrs die verwaltung der phasenwechsel des drehgebers und das erfassen des zeitstempels des auftretens eines phasenwechsels

//für kanal 0
void EXTI0_IRQHandler(void)
{
    //zeitstempel holen
    uint32_t zeitstempel = getTimeStamp();

    EXTI->PR = (1<<0);


 
}


//für kanal 1
void EXTI1_IRQHandler(void)
{
    EXTI->PR = (1<<1);
  
}

//vervollständigen
void get_drehgeberdaten(uint32_t *zeitstempel_out, uint32_t *zaehler_out)
{

}


