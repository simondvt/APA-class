#ifndef STAZIONE_H_INCLUDED
#define STAZIONE_H_INCLUDED

#include <stdio.h>
#define MAX_STR 10

typedef struct stazione
{
    char *stationID;
    int numParcheggi;
    int numPosti;
    int numBici;
    struct stazione *next;
} Stazione;

Stazione *leggiStazione(FILE *fp);
// inserimento in testa nella lista di stazioni
Stazione *insStatione(Stazione *head, Stazione *next);
// ricerca
Stazione *searchStazione(Stazione *head, char *key);

#endif // STAZIONE_H_INCLUDED
