#include "lcd.h"
#include "stm32f429xx.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>
#include "calc.h"

extern volatile bool s6;

extern volatile Phase_t aktuellePhase;
volatile uint32_t flanke_start;


//muss man so nennen, damit CPU interruptfunktion findet
//für button s6
void EXTI9_5_IRQHandler(void)
{
    EXTI->PR = (1<<6);
    s6 = true;
}

void interrupt_handler()
{
    flanke_start = getTimeStamp();

    int port = GPIOG->IDR & 3;
    if      (port == 0)    aktuellePhase = PHASE_A;
    else if (port == 2)    aktuellePhase = PHASE_B;
    else if (port == 3)    aktuellePhase = PHASE_C;
    else                   aktuellePhase = PHASE_D;

    verarbeite_phasenwechsel2();
}

//nach aufgabe verläuft in den beiden isrs die verwaltung der phasenwechsel des drehgebers und das erfassen des zeitstempels des auftretens eines phasenwechsels

//für kanal 0
void EXTI0_IRQHandler(void)
{
    EXTI->PR = (1<<0);
    interrupt_handler();
}

//für kanal 1
void EXTI1_IRQHandler(void)
{
    EXTI->PR = (1<<1);
    interrupt_handler();
}

//vervollständigen
void get_drehgeberdaten(uint32_t *zeitstempel_out, uint32_t *zaehler_out) // wofür weden die Parameter gebraucht?
{
    *zaehler_out = gibPulseCount();
    *zeitstempel_out = flanke_start;
}

void get_drehgeberdaten2()
{

}


