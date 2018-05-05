#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utente.h"

Utente* UTENTEsetvoid(void)
{
    Utente *newU;

    newU = malloc(sizeof(*newU));
    newU->cardID = NULL;
    newU->elenco = NULL;

    return newU;
}

int KEYcompare(Key k1, Key k2)
{
    return strcmp(k1, k2);
}

Key KEYget(Utente *u)
{
    return u->cardID;
}

int UTENTEless(Utente *u1, Utente *u2)
{
    return KEYcompare(KEYget(u1), KEYget(u2)) < 0;
}

void UTENTEshow(Utente *u)
{
    printf("%s\n", KEYget(u));
}

Utente *newUtente(char *cardID, Stazione *head)
{
    Utente *newU;

    newU = malloc(sizeof(*newU));
    newU->cardID = strdup(cardID);
    newU->timeLastNoleggio = NIL;
    newU->elenco = malloc(sizeof(*(newU->elenco)));
    newU->elenco->head = head;
    newU->elenco->numNoleggi = 0;
    newU->elenco->next = NULL;

    return newU;
}

int insElencoStatione(ElencoStazione *head, Stazione *next)
{
    /* head NON è mai NULL */

    ElencoStazione *cur;

    for (cur = head; cur != NULL; cur = cur->next)
    {
        if (cur->head == next)
        {
            return ++cur->numNoleggi;
        }
    }

    // inserimento in coda
    cur = malloc(sizeof(*cur));
    cur->head = next;
    cur->next = NULL;
    cur->numNoleggi = 1;

    return cur->numNoleggi;
}