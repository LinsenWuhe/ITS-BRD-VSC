#include "LED_output.h"
#include "stm32f429xx.h"
#include "calc.h"

void greune_LED_an(int LED) {
    switch (LED) {
        case 23: GPIOE->ODR |= LED_vorwaertsDrehung;  break;
        case 22: GPIOE->ODR |= LED_rueckwaertsDrehung; break;
        case 21: GPIOE->ODR |= LED_fehler;             break;
        default: return;
    }
}

void gruene_LED_aus(int LED) {
    switch (LED) {
        case 23: GPIOE->ODR &= ~LED_vorwaertsDrehung;  break;
        case 22: GPIOE->ODR &= ~LED_rueckwaertsDrehung; break;
        case 21: GPIOE->ODR &= ~LED_fehler;             break;
        default: return;
    }
}

/* Statt einzelner blaue_LED_an/aus Funktionen
   einfach den ganzen Port auf einmal setzen! */
void blaue_LEDs_setzen(int pulse_count)
{
    /* Nur unterste 8 Bits nehmen */
    uint8_t wert = (uint8_t)(pulse_count & 0xFF);

    /* Unterste 8 Bits von GPIOD setzen, Rest unveraendert */
    GPIOD->ODR = (GPIOD->ODR & 0xFF00) | wert;
}

/* Alle LEDs auf einmal aktualisieren */
void updateLEDs(void)
{
    richtung_t richtung = gibRichtung();
    bool       fehler   = gibFehler();
    int        pulse    = gibPulseCount();

    /* Richtungs LEDs */
    if (richtung == RICHTUNG_VOR) {
        greune_LED_an(23);   /* Vorwärts an */
        gruene_LED_aus(22);  /* Rückwärts aus */
    }
    else if (richtung == RICHTUNG_ZUR) {
        gruene_LED_aus(23);  /* Vorwärts aus */
        greune_LED_an(22);   /* Rückwärts an */
    }

    /* Fehler LED */
    if (fehler) {
        greune_LED_an(21);
    }
    else {
        gruene_LED_aus(21);
    }

    /* Blaue LEDs - pulse_count binaer */
    blaue_LEDs_setzen(pulse);
}