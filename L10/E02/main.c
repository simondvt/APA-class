#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stazione.h"
#include "utente.h"
#include "BST.h"

// leggi e ritorna la lista di stazioni
Stazione *leggiStazioni(char *nome);
// funzione lettura tessere
int leggiTessera(BST utenti, Stazione *stazioni, char *cardID, char *stationID, int time);

int main(int argc, char *argv[])
{
    Stazione *stazioni; // lista stazioni
    BST         utenti; // albero utenti
    char cardID[MAX_STR + 1], stationID[MAX_STR + 1];
    int time, durata;

    if (argc != 2)
    {
        printf("Uso: %s <file_stazioni>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    stazioni = leggiStazioni(argv[1]);
    utenti = BSTinit();

    printf("Inserire cardID, stationID e time: ");
    scanf("%s %s %d", cardID, stationID, &time);

    for (;;)
    {
        durata = leggiTessera(utenti, stazioni, cardID, stationID, time);
        if (durata != 0)  // se ho terminato un noleggio
            printf("Durata: %d\n", durata);

        printf("Inserire cardID, stationID e time: ");
        scanf("%s %s %d", cardID, stationID, &time);
    }

    return 0;
}


Stazione *leggiStazioni(char *nome)
{
    FILE *fp = fopen(nome, "r");
    int i, s;
    Stazione *head = NULL;

    fscanf(fp, "%d\n", &s);

    for (i = 0; i < s; i++)
        head = insStatione(head, leggiStazione(fp));

    fclose(fp);
    return head;
}

int leggiTessera(BST utenti, Stazione *stazioni, char *cardID, char *stationID, int time)
{
    Utente   *u;
    Stazione *s;
    int durata, nNoleggi;

    u = BSTsearch(utenti, cardID);
    s = searchStazione(stazioni, stationID);

    if (s == NULL)
    {
        printf("Non esiste la stazione %s\n", stationID);
        exit(EXIT_SUCCESS);
    }

    // se l'utente non esiste lo creo
    if (u == NULL)
    {
        u = newUtente(cardID, s);
        BSTinsert_leafI(utenti, u);
    }

    if (u->timeLastNoleggio == NIL)   // non ho un noleggio attivo
    {
        if (s->numBici > 0)
        {
            s->numBici--;
            s->numParcheggi++;
            u->timeLastNoleggio = time;
            // controllo se s è nella lista, nel caso aumenta il contatore, altrimenti creo un nuovo nodo
            nNoleggi = insElencoStatione(u->elenco, s);

            printf("Noleggio avviato la %d volta da questa stazione.\n", nNoleggi);
        }
        else
        {
            printf("La stazione %s NON ha bici disponibili.\n", s->stationID);
        }
    }
    else    // ho un noleggio attivo
    {
        if (s->numParcheggi > 0)
        {
            durata = time - u->timeLastNoleggio;
            u->timeLastNoleggio = NIL;
            s->numParcheggi--;
            s->numBici++;

            printf("Noleggio terminato.\n");
            return durata;
        }
        else
        {
            printf("La stazione %s NON ha posti liberi.\n", s->stationID);
        }
    }

    return 0;
}

