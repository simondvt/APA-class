#ifndef ESERPIANO_H_INCLUDED
#define ESERPIANO_H_INCLUDED

#include "esercizio.h"

typedef struct eserPiano eserPiano_t, *eserPiano_l;

// costruttore
eserPiano_l newEserPiano(int set, int rip, esercizio_l es, eserPiano_l next);
// distruttore
void freeEserPiano(eserPiano_l esP);

// funzioni getter e setter
int getEserSet(eserPiano_l er);
void setEserSet(eserPiano_l er, int set);
int getEserRip(eserPiano_l er);
void setEserRip(eserPiano_l er, int rip);
esercizio_l getEserEs(eserPiano_l er);
eserPiano_l getEserNext(eserPiano_l er);
void setEserNext(eserPiano_l er, eserPiano_l next);


#endif // ESERPIANO_H_INCLUDED
