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
//ca 200 ms
void EXTI9_5_IRQHandler(void)
{
    EXTI->PR = (1<<6);
    s6 = true;
}

void interrupt_handler()
{
    flanke_start = getTimeStamp();
    aktuellePhase = get_aktuelle_phase();
    verarbeite_phasenwechsel2();
}

//nach aufgabe verläuft in den beiden isrs die verwaltung der phasenwechsel des drehgebers und das erfassen des zeitstempels des auftretens eines phasenwechsels

//für kanal 0
//ca 200ms
void EXTI0_IRQHandler(void)
{

    EXTI->PR = (1<<0);
    interrupt_handler();

    

}

//für kanal 1
// ca 200 ms
void EXTI1_IRQHandler(void)
{
     GPIOG->MODER |= (1 << 4); 	//Bit 4+5 auf Output setzen -> 01
	GPIOG->MODER &= ~(1 << 5);
    //GPIOG->ODR |= (1 << 3);
    GPIOG->BSRR = (1 << 3);

    EXTI->PR = (1<<1);
    interrupt_handler();

    //GPIOG->ODR &= ~(1 << 3);
    GPIOG->BSRR = (1 << (3+16));
}

//vervollständigen
void get_drehgeberdaten(uint32_t *zeitstempel_out, uint32_t *zaehler_out) // wofür weden die Parameter gebraucht?
{
    *zaehler_out = gibPulseCount();
    *zeitstempel_out = flanke_start;
}
