#ifndef TABESER_H_INCLUDED
#define TABESER_H_INCLUDED

#include "esercizio.h"

typedef struct tabEser tabEser_t, *tabEser_l;

struct tabEser
{
    esercizio_l vettEser;
    int nEser;
};

// costruttore
tabEser_l newTabEser(void);
// distruttore
void freeTabEser(tabEser_l te);
// legge i dati da file memorizzandoli in un vettore
// ritorna il numero n di esercizi letti
int leggiEsercizi(tabEser_l te, FILE *fp);
// ricerca
esercizio_l findEsercizio(tabEser_l te, char *nome);

#endif // TABESER_H_INCLUDED
