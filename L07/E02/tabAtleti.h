#ifndef TABATLETI_H_INCLUDED
#define TABATLETI_H_INCLUDED


#include "atleta.h"
#define MAXSTR 25

typedef struct tabAtleti tabAtleti_t, *tabAtleti_l;

struct tabAtleti
{
    int nAtleti;

    atleta_l head, tail;
};


// costruttore
tabAtleti_l newTabAtleti(void);
// distruttore
void freeTabAtleti(tabAtleti_l ta);
// rimuovi un nodo dalla lista a partire dall'id
void removeAtletaFromList(tabAtleti_l a);
// aggiungi nodo in lista
void addAtletaToList(tabAtleti_l ta, atleta_l node);
// legge i dati da file memorizzandoli in una
// ritorna il numero n di atleti letti
int leggiAtleti(tabAtleti_l ta, FILE *fp);

// stampa tutti gli atleti
void stampa(tabAtleti_l ta);
// funzione di confronto per la qsort
int lessCategoria(const void *a1, const void *a2);
// stampa per categoria
void stampaCategoria(tabAtleti_l ta);
// aggiornamento ore
void aggiornaOre(tabAtleti_l ta);
// ricerca per codice
atleta_l codeSearch(tabAtleti_l ta);
// ricerca per cognome
atleta_l nameSearch(tabAtleti_l ta);
// aggiungi nuovo atleta
void addAtleta(tabAtleti_l ta);

#endif // TABATLETI_H_INCLUDED
