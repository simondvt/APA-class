#ifndef UTENTE_H_INCLUDED
#define UTENTE_H_INCLUDED

#include "stazione.h"
#define MAX_STR 10
#define NIL -1

typedef char *Key;

typedef struct es
{
    Stazione *head;     // puntatore all'elemento
    int numNoleggi;
    struct es *next;
} ElencoStazione;

typedef struct
{
    char *cardID;
    int timeLastNoleggio;       // NIL se non c'è un noleggio attivo
    ElencoStazione *elenco;     // lista di elementi della lista stazioni in cui un utente abbia avviato un noleggio
} Utente;

// cerca se next è nella lista, nel caso aumenta il contatore, altrimenti crea il nodo
int insElencoStatione(ElencoStazione *head, Stazione *next);
Utente *newUtente(char *cardID, Stazione *head);

// funzioni per BST
Utente *UTENTEsetvoid(void);
int KEYcompare(Key k1, Key k2);
Key KEYget(Utente *u);
int UTENTEless(Utente *u1, Utente *u2);
void UTENTEshow(Utente *u);

#endif // UTENTE_H_INCLUDED
