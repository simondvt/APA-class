#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabEser.h"

tabEser_l newTabEser(void)
{
    tabEser_l te;

    if ((te = malloc(sizeof(tabEser_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    return te;
}

void freeTabEser(tabEser_l te)
{
    int i;

    for (i = 0; i < te->nEser; i++)
        freeEsercizio(&(te->vettEser[i]));

    free(te);
}

int leggiEsercizi(tabEser_l te, FILE *fp)
{
    char nomeEs[MAXSTR_ESERCIZIO + 1], catEs[MAXSTR_ESERCIZIO + 1], tipoEs[MAXSTR_ESERCIZIO + 1];
    int i, n;
    esercizio_l es;

    fscanf(fp, "%d\n", &n);
    te->nEser = n;

    if ((te->vettEser = malloc(n * sizeof(esercizio_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%s", nomeEs);
        fscanf(fp, "%s", catEs);
        fscanf(fp, "%s", tipoEs);

        es = newEsercizio(nomeEs, catEs, tipoEs);
        te->vettEser[i] = *es;
    }

    return n;
}

esercizio_l findEsercizio(tabEser_l te, char *nome)
{
    int i;

    for (i = 0; i < te->nEser; i++)
        if (strcmp(te->vettEser[i].nome, nome) == 0)
            return &(te->vettEser[i]);

    return NULL;
}


