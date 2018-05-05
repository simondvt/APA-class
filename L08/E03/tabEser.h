#ifndef TABESER_H_INCLUDED
#define TABESER_H_INCLUDED

#include "esercizio.h"
#include "atleta.h"
#include "tabAtleti.h"

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
// carica esercizio nel piano
void loadEsercizio(tabEser_l te, atleta_l a, FILE *fp);
// carica piano di un atleta
void loadPiano(tabAtleti_l ta, tabEser_l te);
// salva piano di un atleta
void savePiano(tabAtleti_l ta);
// stampa il piano
void printPiano(tabAtleti_l ta, atleta_l a);


#endif // TABESER_H_INCLUDED
