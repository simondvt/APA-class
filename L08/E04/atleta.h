#ifndef ATLETA_H_INCLUDED
#define ATLETA_H_INCLUDED

#include "eserPiano.h"
typedef struct atleta atleta_t, *atleta_l;

struct atleta
{
    char *id;
    char *nome;
    char *cognome;
    char *categoria;
    char *data;
    int ore;

    eserPiano_l head, tail;
    atleta_l next;
};

// costruttore
atleta_l newAtleta(char *id, char *nome, char *cognome, char *categoria,
                   char *data, int ore, eserPiano_l head, eserPiano_l tail, atleta_l next);
// distruttore
void freeAtleta(atleta_l a);
// stampa un atleta sullo stream out
void stampaAtleta(atleta_l a, FILE *out);
// aggiungi esercizio in lista
atleta_l addEsercizioToList(atleta_l a, eserPiano_l node);
// rimuovi esercizio in lista
atleta_l removeEsercizioFromList(atleta_l a, char *nome);
// stampa atleta e il piano se disponibile
void stampaAtletaPiano(atleta_l a);


#endif // ATLETA_H_INCLUDED
