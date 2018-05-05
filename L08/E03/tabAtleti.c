#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabAtleti.h"


void addAtletaToList(tabAtleti_l ta, atleta_l node)
{
    atleta_l *head = &(ta->head);
    atleta_l *tail = &(ta->tail);

    if (*head == NULL)
        *head = *tail = node;
    else
    {
        (*tail)->next = node;
        (*tail) = (*tail)->next;
    }
}

void removeAtletaFromList(tabAtleti_l a)
{
    atleta_l x, p;
    char id[MAXSTR + 1];

    printf("Cancella atleta:\n");
    printf("Inserire ID: ");
    scanf("%s", id);

    if (a->head == NULL)
        return;

    for (x = a->head, p = NULL; x != NULL; p = x, x = x->next)
    {
        if (strcmp(id, x->id) == 0)
        {
            if (x == a->head)
                a->head = x->next;
            else
                p->next = x->next;

            freeAtleta(x);
            return;
        }
    }

    printf("Atleta non presente in lista\n");
}


int leggiAtleti(tabAtleti_l ta, FILE *fp)
{
    atleta_l a;
    char id[MAXSTR + 1], nome[MAXSTR + 1], cognome[MAXSTR + 1],
        categoria[MAXSTR + 1], data[MAXSTR + 1];
    int i = 0;
    int dd, mm, yy;
    int ore;

    while (fscanf(fp, "%s", id) != EOF)
    {
        fscanf(fp, "%s", nome);
        fscanf(fp, "%s", cognome);
        fscanf(fp, "%s", categoria);

        fscanf(fp, "%2d/%2d/%4d", &dd, &mm, &yy);
        sprintf(data, "%02d/%02d/%4d", dd, mm, yy);

        fscanf(fp, "%d", &ore);

        a = newAtleta(id, nome, cognome, categoria, data, ore, NULL, NULL, NULL);
        addAtletaToList(ta, a);
        i++;
    }

    ta->nAtleti = i;
    ta->tail = a;

    return ta->nAtleti;
}

void stampa(tabAtleti_l ta)
{
    FILE *out;
    atleta_l a;
    char sOut[MAXSTR + 1];

    printf("Inserire lo stream su cui stampare i dati(vuoto per stdout): ");
    fgets(sOut, MAXSTR, stdin);

    if (sOut[0] == '\n')  // se l'utente lascia vuoto la fgets salva su sOut "\n"
        out = stdout;
    else
    {
        // fopen non può aprire nomi di file che terminano in \n, quindi tronco in quel punto
        sOut[strlen(sOut) - 1] = '\0';

        if ((out = fopen(sOut, "w")) == NULL)
        {
            printf("Impossibile scrivere su %s\n", sOut);
            exit(1);
        }
    }


    for (a = ta->head; a != NULL; a = a->next)
        stampaAtleta(a, out);


    if (out != stdout)
        fclose(out);
}

int lessCategoria(const void *a1, const void *a2)
{
    // a2 e a2 sono di tipo Atleta ** perché così richiede la qsort
    atleta_l *atleta1 = (atleta_l *) a1;
    atleta_l *atleta2 = (atleta_l *) a2;

    return strcmp((*atleta1)->categoria, (*atleta2)->categoria);
}

void stampaCategoria(tabAtleti_l ta)
{
    int i, n;
    char categoria[MAXSTR + 1];
    atleta_l head;

    n = ta->nAtleti;
    // atleti è un vettore di puntatori a struct perché così non spreco memoria e spazio
    // a ricopiare le struct, ma copio solo i puntatori
    atleta_l *atleti = malloc(n * sizeof(atleta_l));

    if (atleti == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    // inizializzo l'array, ogni elemento contiene una struttura atleti
    for (i = 0, head = ta->head; i < n; i++, head = head->next)
        atleti[i] = head;


    // ordino
    qsort(atleti, n, sizeof(atleta_l), lessCategoria);

    strcpy(categoria, atleti[0]->categoria);
    printf("Categoria [%s]:\n", categoria);

    for (i = 0; i < n; i++)
    {
        // se sono passato ad una nuova categoria, stampo il relativo nome
        if (strcmp(atleti[i]->categoria, categoria) != 0)
        {
            strcpy(categoria, atleti[i]->categoria);
            printf("\nCategoria [%s]:\n", categoria);
        }

        stampaAtleta(atleti[i], stdout);
    }

    free(atleti);
}

void aggiornaOre(tabAtleti_l ta)
{
    atleta_l head;
    char code[MAXSTR + 1];
    int ore;

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    for (head = ta->head; head != NULL; head = head->next)
    {
        if (strcmp(code, head->id) == 0) // trovato
        {
            stampaAtleta(head, stdout);

            printf("Inserire ore: ");
            scanf("%d", &ore);
            head->ore = ore;

            break;
        }
    }
}

atleta_l codeSearch(tabAtleti_l ta)
{
    char code[MAXSTR + 1];
    atleta_l head;

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    for (head = ta->head; head != NULL; head = head->next)
        if (strcmp(code, head->id) == 0)
            return head;

    printf("Atleta non trovato!\n\n");
    return NULL;
}

atleta_l nameSearch(tabAtleti_l ta)
{
    atleta_l head;
    char name[MAXSTR + 1];
    int len;

    printf("Inserire il nome dell' atleta: ");
    scanf("%s", name);
    len = strlen(name);

    for (head = ta->head; head != NULL; head = head->next)
        // uso di strncmp per la ricerca parziale
        if (strncmp(name, head->cognome, len) == 0)
            return head;;

    printf("Atleta non trovato!\n\n");
    return NULL;
}

void addAtleta(tabAtleti_l ta)
{
    int dd, mm, yy;
    int ore;
    char id[MAXSTR + 1], nome[MAXSTR + 1], cognome[MAXSTR + 1],
        categoria[MAXSTR + 1], data[MAXSTR + 1];
    atleta_l a;

    printf("Aggiungi un nuovo atleta:\n");
    printf("ID: "); scanf("%s", id);
    printf("Nome: "); scanf("%s", nome);
    printf("Cognome: "); scanf("%s", cognome);
    printf("Categoria: "); scanf("%s", categoria);

    printf("Data: "); scanf("%2d/%2d/%4d", &dd, &mm, &yy);
    sprintf(data, "%02d/%02d/%4d", dd, mm, yy);

    printf("Ore: "); scanf("%d", &ore);

    a = newAtleta(id, nome, cognome, categoria, data, ore, NULL, NULL, NULL);
    addAtletaToList(ta, a);
}

void freeTabAtleti(tabAtleti_l ta)
{
    atleta_l a, tmp;

    for (a = ta->head; a != NULL; a = a->next)
    {
        tmp = a;
        a = a->next;
        freeAtleta(tmp);
    }

    free(ta);
}

tabAtleti_l newTabAtleti(void)
{
    tabAtleti_l ta;

    if ((ta = malloc(sizeof(tabAtleti_t))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    ta->head = ta->tail = NULL;

    return ta;
}

void editPiano(tabAtleti_l ta, esercizio_l e, char *nome)
{
    atleta_l a;
    eserPiano_l es;
    int newSet, newRip;
    int scelta;

    if ((a = codeSearch(ta)) == NULL)
    {
        printf("Piano atleta non trovato\n");
        exit(EXIT_FAILURE);
    }

    printf("\n1 - Aggiungi Esercizio\n2 - Cancella Esercizio\n3 - Modifica set e rip: ");
    scanf("%d", &scelta);

    switch (scelta)
    {
    case 1:
        printf("Inserisci rip e set: ");
        scanf("%d %d", &newRip, &newSet);

        es = newEserPiano(newSet, newRip, e, NULL);
        addEsercizioToList(a, es);
        break;
    case 2:
        a = removeEsercizioFromList(a, nome);
        break;
    case 3:
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
        break;
    default: printf("Scelta non valida\n\n");
    }
}
