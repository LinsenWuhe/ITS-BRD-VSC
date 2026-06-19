#ifndef MASTER_H
#define MASTER_H

//ist jetzt 0 - eigentlich unnötig, aber wenn pin mal geändert wird, muss es nur hier geändert werden
#include <stdint.h>
#define PIN 0

//zum lesen und schreiben von master

void sende1();

void sende0();

int liesBit();

int reset();

void sendeByte(uint8_t byte);

uint8_t liesByte();

uint8_t berechneCRC(uint8_t *daten, int laenge);

void starteSucheNeu();

int sucheNaechstenSensor(uint8_t *id_buffer);



#endif