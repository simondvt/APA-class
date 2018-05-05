#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atleta.h"

atleta_l newAtleta(char *id, char *nome, char *cognome, char *categoria,
                   char *data, int ore, eserPiano_l head, eserPiano_l tail, atleta_l next)
{
    atleta_l a;

    if ((a = malloc(sizeof(atleta_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    if ((a->id = malloc((strlen(id) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(a->id, id);

    if ((a->nome = malloc((strlen(nome) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(a->nome, nome);

    if ((a->cognome = malloc((strlen(cognome) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(a->cognome, cognome);

    if ((a->categoria = malloc((strlen(categoria) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(a->categoria, categoria);

    if ((a->data = malloc((strlen(data) + 1) * sizeof(char))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    strcpy(a->data, data);

    a->ore = ore;
    a->head = head;
    a->tail = tail;
    a->next = next;

    return a;
}

void freeAtleta(atleta_l a)
{
    free(a->id);
    free(a->nome);
    free(a->cognome);
    free(a->categoria);
    free(a->data);

    free(a);
}

void stampaAtleta(atleta_l a, FILE *out)
{
    fprintf(out, "ID: %s\n", a->id);
    fprintf(out, "Nome: %s\n", a->nome);
    fprintf(out, "Cognome: %s\n", a->cognome);
    fprintf(out, "Categoria: %s\n", a->categoria);
    fprintf(out, "Data: %s\n", a->data);
    fprintf(out, "Ore: %d\n\n", a->ore);
}


void addEsercizioToList(atleta_l a, eserPiano_l node)
{
    eserPiano_l *head = &(a->head);
    eserPiano_l *tail = &(a->tail);

    if (*head == NULL)
        *head = *tail = node;
    else
    {
        (*tail)->next = node;
        (*tail) = (*tail)->next;
    }
}


