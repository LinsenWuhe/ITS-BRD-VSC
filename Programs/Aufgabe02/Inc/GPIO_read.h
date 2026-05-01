#ifndef GPIO_READ_H
#define GPIO_READ_H

#define HIGH 1
#define LOW  0

// Nur deklarieren mit extern 
extern int kanal1;
extern int kanal2;

int status_drehscheibe(void);
int s6_lesen(void);

#endif