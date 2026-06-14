#ifndef MASTER_H
#define MASTER_H

//ist jetzt 0 - eigentlich unnötig, aber wenn pin mal geändert wird, muss es nur hier geändert werden
#define PIN 0

//zum lesen und schreiben von master

void sende1();

void sende0();

int liesBit();

int reset();
#endif