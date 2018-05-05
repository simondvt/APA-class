#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabEser.h"

struct tabEser
{
    esercizio_l vettEser;
    int nEser;
};

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

void loadEsercizio(tabEser_l te, atleta_l a, FILE *fp)
{
    int set;
    int rip;
    char nome[MAXSTR_ESERCIZIO + 1];
    eserPiano_l es;
    esercizio_l e;

    while (fscanf(fp, "%s", nome) != EOF)
    {
        fscanf(fp, "%d %d\n", &set, &rip);
        e = findEsercizio(te, nome);

        es = newEserPiano(set, rip, e, NULL);
        a = addEsercizioToList(a, es);
    }

    fclose(fp);
}

void loadPiano(tabAtleti_l ta, tabEser_l te)
{
    FILE *fp;
    atleta_l a = codeSearch(ta);
    char path[MAXSTR + 4];

    if (a == NULL)
    {
        printf("Piano atleta non trovato\n");
        exit(EXIT_FAILURE);
    }

    strcpy(path, a->id);
    strcat(path, ".txt");

    printf("Carica piano di un atleta:\n");
    if ((fp = fopen(path, "r")) == NULL)
    {
        printf("Piano non trovato\n");
        exit(EXIT_FAILURE);
    }

    loadEsercizio(te, a, fp);
    printPiano(ta, a);
}

void savePiano(tabAtleti_l ta)
{
    atleta_l a;
    FILE *fp;
    eserPiano_l es;
    char path[MAXSTR + 4];

    printf("Salva piano di un atleta: \n");
    a = codeSearch(ta);

    if (a == NULL)
    {
        printf("Piano atleta non trovato\n");
        exit(EXIT_FAILURE);
    }

    strcpy(path, a->id);
    strcat(path, ".txt");

    if ((fp = fopen(path, "w")) == NULL)
    {
        printf("Impossibile salvare piano atleta\n");
        exit(EXIT_FAILURE);
    }

    if (a->head == NULL)
    {
        printf("Piano non ancora caricato\n");
        exit(EXIT_FAILURE);
    }

    for (es = a->head; es != NULL; es = getEserNext(es))
        fprintf(fp, "%s %d %d\n", getEserEs(es)->nome, getEserSet(es), getEserRip(es));

    fclose(fp);
}

void printPiano(tabAtleti_l ta, atleta_l a)
{
    eserPiano_l es;

    if (a == NULL)
    {
        a = codeSearch(ta);

        if (a == NULL)
        {
            printf("Piano atleta non trovato\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Nome\tSet\tRip\n\n");
    for (es = a->head; es != NULL; es = getEserNext(es))
        printf("%s\t%d\t%d\n", getEserEs(es)->nome, getEserSet(es), getEserRip(es));
}

esercizio_l getArrayEser(tabEser_l te)
{
    return te->vettEser;
}

int getNEser(tabEser_l te)
{
    return te->nEser;
}
