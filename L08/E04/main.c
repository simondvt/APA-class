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


// legge i dati da file e si interfaccia con l'utente
// mediante un menu basato su intero
int main(void)
{
    atleta_l a;
    tabAtleti_l ta;
    tabEser_l te;
    esercizio_l e;
    int scelta; // scelta del menu
    FILE *atleti_fp, *esercizi_fp;
    char nome[MAXSTR_ESERCIZIO + 1];

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
    printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n8 - Carica piano atleta\n9 - Salva piano atleta\n10 - Modifica esercizio del piano\n11 - Stampa Atleta e piano\n12 - Stampa dettagli esercizio\n");
    scanf("%d", &scelta);
    do
    {
        fflush(stdin);

        switch (scelta)
        {
            case 0: freeTabAtleti(ta); freeTabEser(te);                         return 0;
            case 1: stampa(ta);                                                 break;
            case 2: stampaCategoria(ta);                                        break;
            case 3: aggiornaOre(ta);                                            break;
            case 4: stampaAtleta(codeSearch(ta), stdout);                       break;
            case 5: stampaAtleta(nameSearch(ta), stdout);                       break;
            case 6: addAtleta(ta);  setNAtleti(ta, getNAtleti(ta) + 1);             break;
            case 7: removeAtletaFromList(ta); setNAtleti(ta, getNAtleti(ta) - 1);   break;
            case 8: loadPiano(ta, te);                                          break;
            case 9: savePiano(ta);                                              break;
            case 10: printf("Inserisci nome esercizio: ");
                     scanf("%s", nome);
                     e = findEsercizio(te, nome); editPiano(ta, e, nome);       break;
            case 11: a = codeSearch(ta);    stampaAtletaPiano(a);               break;
            case 12: printf("Inserisci nome esercizio: ");
                     scanf("%s", nome);
                     stampaEsercizio(findEsercizio(te, nome));                  break;

            default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n8 - Carica piano atleta\n9 - Salva piano atleta\n10 - Modifica esercizio del piano\n11 - Stampa Atleta e piano\n12 - Stampa dettagli esercizio\n");
        scanf("%d", &scelta);
    } while (scelta != 0);
    return 0;
}
