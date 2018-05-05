#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stazione.h"

Stazione *leggiStazione(FILE *fp)
{
    char stationID[MAX_STR + 1];    // buffer
    int numPosti, numBici;
    Stazione *newS;

    fscanf(fp, "%s %d %d", stationID, &numBici, &numPosti);

    newS = malloc(sizeof(*newS));
    newS->stationID = strdup(stationID);
    newS->numBici = numBici;
    newS->numPosti = numPosti;
    newS->numParcheggi = numPosti - numBici;
    newS->next = NULL;

    return newS;
}

Stazione *insStatione(Stazione *head, Stazione *next)
{
    if (head == NULL)
        return next;

    next->next = head;
    return next;
}

Stazione *searchStazione(Stazione *head, char *key)
{
    Stazione *cur;

    for (cur = head; cur != NULL; cur = cur->next)
        if (strcmp(cur->stationID, key) == 0)
            return cur;

    return NULL;
}