#ifndef LED_output_H
#define LED_output_H

#define LED_fehler              (1 << 5)    //LED D21
#define LED_rueckwaertsDrehung  (1 << 6)    //LED D22
#define LED_vorwaertsDrehung     (1 << 7)    //LED D23

void greune_LED_an(int LED);

void gruene_LED_aus(int LED);

void blaue_LEDs_setzen(int LED);

void updateLEDs(void);


#endif