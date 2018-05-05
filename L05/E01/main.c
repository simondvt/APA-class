#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRCFILE "atleti.txt"
#define MAXSTR 25

// constanti usate nel descrivere lo stato di ordinamento
typedef enum {NULLA, CODICE, COGNOME, CATEGORIA} status;

typedef struct
{
    char *id;
    char *nome;
    char *cognome;
    char *categoria;
    char *data;
    int ore;
} Atleta;

// legge i dati da file memorizzandoli nella struttura atleti
// ritorna il numero n di atleti letti
int leggi(Atleta **atleti);

// ordinamento del vettore basato su Insertion Sort per garantire la stabilità
// uso di un puntatore a funzione per il confronto
void sort(Atleta *atleti, int n, int less(Atleta, Atleta));

// confronta le date di nascita dei due atleti
// ritorna 0 se a1.data >= a2.data
int lessData(Atleta a1, Atleta a2);

// confronta i codice dei due atleti
// ritorna 0 se a1.id >= a2.id
int lessID(Atleta a1, Atleta a2);

// confronta i cognomi dei due atleti
// ritorna 0 se a1.cognome >= a2.cognome in senso alfabetico
int lessCognome(Atleta a1, Atleta a2);

// confronta le categoria dei due atleti
// funzione usata nella stampa per categoria
int lessCategoria(Atleta a1, Atleta a2);

// stampa (a video o file) i contenuti dell'anagrafica
void stampa(Atleta *atleti, int n);

// stampa gli atleti divisi per categoria
void stampaCategoria(Atleta *atleti, int n);

// stampa un singolo atleta sullo stream out
void stampaAtleta(Atleta atleta, FILE *out);

// ricerca di un atleta per codice
// ritorna -1 se non lo trova, altrimenti ritorna l'indice
int codeSearch(Atleta *atleti, int n, status stato);

// ricerca di un atleta per cognome
// ritorna -1 se non lo trova, altrimenti ritorna l'indice
int nameSearch(Atleta *atleti, int n, status stato);

// ricerca l'atleta e poi modifica il suo monte ore
void aggiornaOre(Atleta *atleti, int n, status stato);

// libera la memoria
void destroy(Atleta *atleti, int n);


// legge i dati da file e si interfaccia con l'utente
// mediante un menu basato su intero
int main(void)
{
    Atleta *atleti;
    int n;      // numero di atleti letti
    int scelta; // scelta del menu
    int i;      // indice di ritorno delle ricerche
    status stato;   // tiene conto di che ordinamento usare

    stato = NULLA;
    n = leggi(&atleti);

    printf("Dati letti con successo.\n");
    printf("Scegliere opzione:\n");
    printf("0 - Esci\n1 - Stampa\n2 - Ordinamento basato su data\n3 - Ordinamento basato su codice\n4 - Ordinamento basato su cognome\n5 - Stampa per categoria\n6 - Aggiornamento ore settimanali\n7 - Ricerca per codice\n8 - Ricerca per cognome\n");
    scanf("%d", &scelta);
    do
    {
        fflush(stdin);

        // lo stato viene aggiornato ogni volta che si richiama una
        // funzione che cambia l'ordinamento
        switch (scelta)
        {
            case 0: destroy(atleti, n);                                         return 0;
            case 1: stampa(atleti, n);                                          break;
            case 2: stato = NULLA;          sort(atleti, n, lessData);          break;
            case 3: stato = CODICE;         sort(atleti, n, lessID);            break;
            case 4: stato = COGNOME;        sort(atleti, n, lessCognome);       break;
            case 5: stato = CATEGORIA;      stampaCategoria(atleti, n);         break;
            case 6: aggiornaOre(atleti, n, stato);                              break;

            // passo n - 1 perché la ricerca è basata sugli indici l e r
            case 7: i = codeSearch(atleti, n - 1, stato);
                    if (i != -1) stampaAtleta(atleti[i], stdout);
                    break;
            case 8: i = nameSearch(atleti, n - 1, stato);
                    if (i != -1) stampaAtleta(atleti[i], stdout);
                    break;
					
			default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa\n2 - Ordinamento basato su data\n3 - Ordinamento basato su codice\n4 - Ordinamento basato su cognome\n5 - Stampa per categoria\n6 - Aggiornamento ore settimanali\n7 - Ricerca per codice\n8 - Ricerca per cognome\n");
        scanf("%d", &scelta);
    } while (scelta != 0);

    return 0;
}


int leggi(Atleta **atleti)
{
    char buf[MAXSTR + 1];
    int i, n;
    FILE *fp;

    int dd, mm, yy;

    if ((fp = fopen(SRCFILE, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", SRCFILE);
        exit(1);
    }

    fscanf(fp, "%d", &n);

    if ((*atleti = malloc(n * sizeof(**atleti))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%s", buf);
        (*atleti)[i].id = malloc((strlen(buf) + 1) * sizeof(*(*atleti)[i].id));
        strcpy((*atleti)[i].id, buf);

        fscanf(fp, "%s", buf);
        (*atleti)[i].nome = malloc((strlen(buf) + 1) * sizeof(*(*atleti)[i].nome));
        strcpy((*atleti)[i].nome, buf);

        fscanf(fp, "%s", buf);
        (*atleti)[i].cognome = malloc((strlen(buf) + 1) * sizeof(*(*atleti)[i].cognome));
        strcpy((*atleti)[i].cognome, buf);

        fscanf(fp, "%s", buf);
        (*atleti)[i].categoria = malloc((strlen(buf) + 1) * sizeof(*(*atleti)[i].categoria));
        strcpy((*atleti)[i].categoria, buf);

        fscanf(fp, "%2d/%2d/%4d", &dd, &mm, &yy);
        (*atleti)[i].data = malloc((10 + 1) * sizeof(*(*atleti)[i].data)); // dd/mm/yyyy sono 10 caratteri
        sprintf((*atleti)[i].data, "%02d/%02d/%4d", dd, mm, yy);

        fscanf(fp, "%d", &((*atleti)[i].ore));
    }

    fclose(fp);
    return n;
}


void stampa(Atleta *atleti, int n)
{
    FILE *out;
    char sOut[MAXSTR + 1];
    int i;

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


    for (i = 0; i < n; i++)
        stampaAtleta(atleti[i], out);


    if (out != stdout)
        fclose(out);
}


void sort(Atleta *atleti, int n, int less(Atleta, Atleta))
{
    int i, j;
    Atleta x;

    for (i = 1; i < n; i++)
    {
        x = atleti[i];
        j = i - 1;

        while (j >= 0 && less(x, atleti[j]))
        {
            atleti[j + 1] = atleti[j];
            j--;
        }

        atleti[j + 1] = x;
    }
}

int lessData(Atleta a1, Atleta a2)
{
    int dd, mm, yy;

    char *data1 = malloc((strlen(a1.data) + 1) * sizeof(*data1));
    char *data2 = malloc((strlen(a2.data) + 1) * sizeof(*data2));

    sscanf(a1.data, "%2d/%2d/%4d", &dd, &mm, &yy);
    sprintf(data1, "%4d/%02d/%02d", yy, mm, dd);

    sscanf(a2.data, "%2d/%2d/%4d", &dd, &mm, &yy);
    sprintf(data2, "%4d/%02d/%02d", yy, mm, dd);

    return strcmp(data1, data2) < 0;
}

int lessID(Atleta a1, Atleta a2)
{
    return strcmp(a1.id, a2.id) < 0;
}

int lessCognome(Atleta a1, Atleta a2)
{
    return strcmp(a1.cognome, a2.cognome) < 0;
}

int lessCategoria(Atleta a1, Atleta a2)
{
    return strcmp(a1.categoria, a2.categoria) < 0;
}

void stampaCategoria(Atleta *atleti, int n)
{
    char categoria[MAXSTR + 1];

    // riordino l'array per categoria
    sort(atleti, n, lessCategoria);

    int i;

    strcpy(categoria, atleti[0].categoria);
    printf("Categoria [%s]:\n", categoria);

    for (i = 0; i < n; i++)
    {
        // se sono passato ad una nuova categoria, stampo il relativo nome
        if (strcmp(atleti[i].categoria, categoria) != 0)
        {
            strcpy(categoria, atleti[i].categoria);
            printf("\nCategoria [%s]:\n", categoria);
        }

        stampaAtleta(atleti[i], stdout);
    }
}


void stampaAtleta(Atleta atleti, FILE *out)
{
    fprintf(out, "ID: %s\n", atleti.id);
    fprintf(out, "Nome: %s\n", atleti.nome);
    fprintf(out, "Cognome: %s\n", atleti.cognome);
    fprintf(out, "Categoria: %s\n", atleti.categoria);
    fprintf(out, "Data: %s\n", atleti.data);
    fprintf(out, "Ore: %d\n\n", atleti.ore);
}


int codeSearch(Atleta *atleti, int n, status stato)
{
    char code[MAXSTR + 1];
    int i, l;

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    if (stato == CODICE)    // ricerca binaria
    {
        l = 0;

        while (l <= n)
        {
            i = l + (n - l) / 2;

            if (strcmp(atleti[i].id, code) == 0)
                return i;

            if (strcmp(atleti[i].id, code) < 0)
                l = i + 1;
            else
                n = i - 1;
        }


        printf("Atleta non trovato!\n");
        return -1;
    }
    else    // ricerca lineare
    {
        for (i = 0; i <= n; i++)
            if (strcmp(code, atleti[i].id) == 0)
                return i;


        printf("Atleta non trovato!\n");
        return -1;
    }
}


int nameSearch(Atleta *atleti, int n, status stato)
{
    char name[MAXSTR + 1];
    int i, l;

    printf("Inserire il nome dell' atleta: ");
    scanf("%s", name);

    if (stato == COGNOME)   // ricerca binaria
    {
        l = 0;

        while (l <= n)
        {
            i = l + (n - l) / 2;

            if (strcmp(atleti[i].cognome, name) == 0)
                return i;

            if (strcmp(atleti[i].cognome, name) < 0)
                l = i + 1;
            else
                n = i - 1;
        }


        printf("Atleta non trovato!\n");
        return -1;
    }
    else    // ricerca lineare
    {
        for (i = 0; i <= n; i++)
            if (strcmp(name, atleti[i].cognome) == 0)
                return i;


        printf("Atleta non trovato!\n");
        return -1;
    }
}


void aggiornaOre(Atleta *atleti, int n, status stato)
{
    // trovo l'atleta da modificare tramite il suo codice
    int i = codeSearch(atleti, n, stato);
    int ore;

    stampaAtleta(atleti[i], stdout);

    printf("Inserire ore: ");
    scanf("%d", &ore);

    atleti[i].ore = ore;
}


void destroy(Atleta *atleti, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        free(atleti[i].id);
        free(atleti[i].nome);
        free(atleti[i].cognome);
        free(atleti[i].categoria);
        free(atleti[i].data);
    }

    free(atleti);
}
