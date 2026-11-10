#ifndef MASTER_H
#define MASTER_H

//ist jetzt 0 - eigentlich unnötig, aber wenn pin mal geändert wird, muss es nur hier geändert werden
#include <stdint.h>
#define PIN 0

#define read_Scratchpad     0xBE //daten auslesen vom Sensor
#define match_ROM           0x55 //mit dem Sensor sprechen, der die ID ...... hat
#define skip_ROM            0xCC // es ist egal, wer welche ID hat, lässt alle Sensoren zuhören
#define convert_t           0x44 //startbefehl für temperaturmessung
#define search_ROM          0xF0 //startet suchverfahren für ids
#define read_ROM            0x33 //ID lesen von EINEM angeschlossenen Sensor

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