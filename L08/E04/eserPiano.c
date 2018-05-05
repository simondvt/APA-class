#include <stdio.h>
#include <stdlib.h>

#include "eserPiano.h"
#include "atleta.h"

struct eserPiano
{
    int set;
    int rip;

    esercizio_l es;
    eserPiano_l next;
};

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

int getEserSet(eserPiano_l er)
{
    return er->set;
}

void setEserSet(eserPiano_l er, int set)
{
    er->set = set;
}

int getEserRip(eserPiano_l er)
{
    return er->rip;
}

void setEserRip(eserPiano_l er, int rip)
{
    er->rip = rip;
}

esercizio_l getEserEs(eserPiano_l er)
{
    return er->es;
}

eserPiano_l getEserNext(eserPiano_l er)
{
    return er->next;
}

void setEserNext(eserPiano_l er, eserPiano_l next)
{
    er->next = next;
}
