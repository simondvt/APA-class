#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esercizio.h"

esercizio_l newEsercizio(char *nome, char *catEs, char *tipoEs)
{
    esercizio_l es;

    if ((es = malloc(sizeof(esercizio_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    if ((es->nome = malloc((strlen(nome) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(es->nome, nome);

    if ((es->catEs = malloc((strlen(catEs) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(es->catEs, catEs);

    if ((es->tipoEs = malloc((strlen(tipoEs) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(es->tipoEs, tipoEs);

    return es;
}

void freeEsercizio(esercizio_l es)
{
    free(es->nome);
    free(es->catEs);
    free(es->tipoEs);

    free(es);
}

void stampaEsercizio(esercizio_l es)
{
    printf("Nome: %s\n", es->nome);
    printf("Categoria: %s\n", es->catEs);
    printf("Tipo: %s\n", es->tipoEs);
}
