#include "ISR_init.h"
#include "stm32f429xx.h"



void s6_isr_init()
{
    //Module aufwecken und mit Strom versorgen
    //RCC (Reset and Clock Control) - einschalten der takte
    //AHB1ENR (Advanced high-performance bus 1 enable register) -> In dem Register hat jeder GPIO-Port sein eigenes Bit
    //verodern (nur 1 bit auf 1 setzen) mit vorgegeber RCC_maske für GPIOF Port
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;    

    //Advances Perpheral Bus 2 Enable Register, wo Syscfg liegt
    //schaltet multiplexer an, da er entscheidet, welcher pin auf welchen exti kanal geleitet wird (sitzt in syscfg) -> takt für mux anschalten
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   

    //schalter 6 als input, falls noch nicht geschehen (00) für input mode 
    GPIOF->MODER &= ~(0x03 << (2 * 6)); 

   
    //EXTI6 soll genau auf Port f hören
    //EXTICR (external interrupt control register) verwaltet 16 interrupt kanäle - insgesamt 16 kanäle, aber ein einzelnes register ist 32 bits groß -> aufgeteilt auf vier register EXTICR[0] bis EXTICR[3] - also 0: kanäle 1,2,3,4 - 1: 5,6,7,8, usw. 
    SYSCFG->EXTICR[1] &= ~(0x0F << (4 * 2)); // Alten Wert für Pin 6 löschen - (4*2) ermittelt 2. Stelle, im 2. Array -> Pin 6, da jeder Kanal 4 Bits breit ist. 
    SYSCFG->EXTICR[1] |=  (0x05 << (4 * 2)); // 0x05 steht beim STM32 für Port F nach Folien - Nimmt 5 und schiebt sie um 8 stellen nach links für kanal 6

    
    //Worauf soll prozessor reagieren - hier: fallende Flanke von button (wird gedrückt - active low)
    EXTI->FTSR |= (1 << 6);  // Falling Trigger Selection Register: Bit 6 setzen
  
    
    // Interrupt und Flag aktivieren -> Interrupt-Maske im EXTI öffnen (Kanal unmaskieren)
    EXTI->IMR |= (1 << 6);   // Interrupt Mask Register: Bit 6 erlauben

    //interrupt kommt bei NVIC (nested vectored interrupt controller) an - NVIC muss interrupt zur CPU durchlassen -> schalter für interrupt kanal muss freigeschaltet werden -> hier nummer 23 nach der Maske
    //cpu weiß nun: "wenn auf leitung 23 ein signal kommt, springe zu der funktion exti9_5_IRQHandler"
    NVIC_EnableIRQ(EXTI9_5_IRQn);

}