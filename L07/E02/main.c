#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atleta.h"
#include "esercizio.h"
#include "eserPiano.h"
#include "tabAtleti.h"
#include "tabEser.h"

#define FILE_ATLETI   "atleti.txt"
#define FILE_ESERCIZI "esercizi.txt"


// carica esercizio nel piano
void loadEsercizio(tabEser_l te, atleta_l a, FILE *fp);
// carica piano di un atleta
void loadPiano(tabAtleti_l ta, tabEser_l te);
// salva piano di un atleta
void savePiano(tabAtleti_l ta);
// stampa il piano
void printPiano(tabAtleti_l ta, atleta_l a);
// modifica un esercizio
void editPiano(tabAtleti_l ta);


// legge i dati da file e si interfaccia con l'utente
// mediante un menu basato su intero
int main(void)
{
    tabAtleti_l ta;
    tabEser_l te;
    int scelta; // scelta del menu
    FILE *atleti_fp, *esercizi_fp;

    if ((atleti_fp = fopen(FILE_ATLETI, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", FILE_ATLETI);
        exit(EXIT_FAILURE);
    }
    if ((esercizi_fp = fopen(FILE_ESERCIZI, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", FILE_ESERCIZI);
        exit(EXIT_FAILURE);
    }

    ta = newTabAtleti();
    te = newTabEser();

    leggiAtleti(ta, atleti_fp);
    leggiEsercizi(te, esercizi_fp);

    fclose(atleti_fp);
    fclose(esercizi_fp);

    printf("Scegliere opzione:\n");
    printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n8 - Carica piano atleta\n9 - Salva piano atleta\n10 - Modifica esercizio del piano\n");
    scanf("%d", &scelta);
    do
    {
        fflush(stdin);

        switch (scelta)
        {
            case 0: freeTabAtleti(ta); freeTabEser(te);                return 0;
            case 1: stampa(ta);                                        break;
            case 2: stampaCategoria(ta);                               break;
            case 3: aggiornaOre(ta);                                   break;
            case 4: stampaAtleta(codeSearch(ta), stdout);              break;
            case 5: stampaAtleta(nameSearch(ta), stdout);              break;
            case 6: addAtleta(ta);               (ta->nAtleti)++;      break;
            case 7: removeAtletaFromList(ta);    (ta->nAtleti)--;      break;
            case 8: loadPiano(ta, te);                                 break;
            case 9: savePiano(ta);                                     break;
            case 10: editPiano(ta);                                    break;

            default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n8 - Carica piano atleta\n9 - Salva piano atleta\n10 - Modifica esercizio del piano\n");
        scanf("%d", &scelta);
    } while (scelta != 0);
    return 0;
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
        addEsercizioToList(a, es);
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

    for (es = a->head; es != NULL; es = es->next)
        fprintf(fp, "%s %d %d\n", es->es->nome, es->set, es->rip);

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
    for (es = a->head; es != NULL; es = es->next)
        printf("%s\t%d\t%d\n", es->es->nome, es->set, es->rip);
}

void editPiano(tabAtleti_l ta)
{
    atleta_l a;
    eserPiano_l es;
    int newSet, newRip;
    char nome[MAXSTR_ESERCIZIO + 1];

    if ((a = nameSearch(ta)) == NULL)
    {
        printf("Piano atleta non trovato\n");
        exit(EXIT_FAILURE);
    }

    printf("Inserisci nome esercizio: ");
    scanf("%s", nome);

    for (es = a->head; es != NULL; es = es->next)
    {
        if (strcmp(es->es->nome, nome) == 0)
        {
            printf("Inserire set e rip: ");
            scanf("%d %d", &newSet, &newRip);

            es->set = newSet;
            es->rip = newRip;

            return;
        }
    }

    printf("Esercizio non trovato\n");
}
