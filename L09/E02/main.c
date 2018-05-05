#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "item.h"
#include "pq.h"

#define MAX 100

void menu(void)
{
    printf("(0) - Esci\n");
    printf("(1) - Stampa stato classifica\n");
    printf("(2) - Inserisci nuovo partecipante\n");
    printf("(3) - Cancella partecipante\n");
    printf("(4) - Evoluzione classifica\n");
    printf("(5) - Carica dati da file\n");
    printf("(6) - Salva dati su file\n");
}

void gioca(PQ pq);
void caricaFile(PQ pq);
void salvaFile(PQ pq);

int main(void)
{
    PQ pq;
    int scelta;

    pq = PQinit(MAX);

    menu();
    scanf("%d", &scelta);

    for (;;)
    {
        switch (scelta)
        {
            case 0: return 0;                                    break;
            case 1: PQshow(pq);                                  break;
            case 2: printf("\nInserire nome partecipante: ");
                    PQinsert(pq, ITEMscan(stdin));               break;
            case 3: PQdelete(pq);                                break;
            case 4: gioca(pq);                                   break;
            case 5: caricaFile(pq);                              break;
            case 6: salvaFile(pq);                               break;
        }

        menu();
        fflush(stdin);
        scanf("%d", &scelta);
    }

    return 0;
}
