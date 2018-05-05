#include <stdio.h>
#include <stdlib.h>
#include "eserPiano.h"
#include "atleta.h"

eserPiano_l newEserPiano(int set, int rip, esercizio_l es, eserPiano_l next)
{
    eserPiano_l esP;

    if ((esP = malloc(sizeof(eserPiano_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    esP->set = set;
    esP->rip = rip;
    esP->es = es;
    esP->next = next;

    return esP;
}

void freeEserPiano(eserPiano_l esP)
{
    free(esP);
}


