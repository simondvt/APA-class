#ifndef ESERPIANO_H_INCLUDED
#define ESERPIANO_H_INCLUDED

#include "esercizio.h"

typedef struct eserPiano eserPiano_t, *eserPiano_l;

struct eserPiano
{
    int set;
    int rip;

    esercizio_l es;
    eserPiano_l next;
};

// costruttore
eserPiano_l newEserPiano(int set, int rip, esercizio_l es, eserPiano_l next);
// distruttore
void freeEserPiano(eserPiano_l esP);


#endif // ESERPIANO_H_INCLUDED
