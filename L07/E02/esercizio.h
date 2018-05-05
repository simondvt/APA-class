#ifndef ESERCIZIO_H_INCLUDED
#define ESERCIZIO_H_INCLUDED

#define MAXSTR_ESERCIZIO 50

typedef struct esercizio esercizio_t, *esercizio_l;

struct esercizio
{
    char *nome;
    char *catEs;
    char *tipoEs;
};

// costruttore
esercizio_l newEsercizio(char *nome, char *catEs, char *tipoEs);
// distruttore
void freeEsercizio(esercizio_l es);

#endif // ESERCIZIO_H_INCLUDED
