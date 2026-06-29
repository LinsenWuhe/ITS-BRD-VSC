#include "ISR_init.h"
#include "stm32f429xx.h"



void s6_isr_init()
{

//so funktioniert es, abgeschrieben von FOlie, aber VERSTEHEN!

RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;    // Takt für Port F an
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   // Takt für System Konfiguration (MUX) an


    GPIOF->MODER &= ~(0x03 << (2 * 6)); 

   
    SYSCFG->EXTICR[1] &= ~(0x0F << (4 * 2)); // Alten Wert für Pin 6 löschen
    SYSCFG->EXTICR[1] |=  (0x05 << (4 * 2)); // 0x05 steht beim STM32 für Port F!

    
    EXTI->FTSR |= (1 << 6);  // Rising Trigger Selection Register: Bit 6 setzen
  
    
    // 5. Interrupt-Maske im EXTI öffnen (Kanal unmaskieren)
    EXTI->IMR |= (1 << 6);   // Interrupt Mask Register: Bit 6 erlauben

    // 6. Den Interrupt im Core-Controller (NVIC) freischalten
    NVIC_EnableIRQ(EXTI9_5_IRQn);

}