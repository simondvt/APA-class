#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRCFILE "atleti.txt"
#define MAXSTR 25

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
void sort(Atleta **atleti, int n, int less(Atleta *, Atleta *));

// confronta le date di nascita dei due atleti
// ritorna 0 se a1.data >= a2.data
int lessData(Atleta *a1, Atleta *a2);

// confronta i codice dei due atleti
// ritorna 0 se a1.id >= a2.id
int lessID(Atleta *a1, Atleta *a2);

// confronta i cognomi dei due atleti
// ritorna 0 se a1.cognome >= a2.cognome in senso alfabetico
int lessCognome(Atleta *a1, Atleta *a2);

// confronta le categoria dei due atleti
// funzione usata nella stampa per categoria
int lessCategoria(Atleta *a1, Atleta *a2);

// stampa (a video o file) i contenuti dell'anagrafica
void stampa(Atleta *atleti, int n);

// stampa gli atleti divisi per categoria
void stampaCategoria(Atleta **atleti, int n);

// stampa un singolo atleta sullo stream out
void stampaAtleta(Atleta atleta, FILE *out);

// ricerca di un atleta per codice
// ritorna -1 se non lo trova, altrimenti ritorna l'indice
int codeSearch(Atleta **atleti, int n);

// ricerca di un atleta per cognome
// ritorna -1 se non lo trova, altrimenti ritorna l'indice
int nameSearch(Atleta **atleti, int n);

// ricerca l'atleta e poi modifica il suo monte ore
void aggiornaOre(Atleta **atleti, int n);

// libera la memoria
void destroy(Atleta *atleti, Atleta **atletiData, Atleta **atletiID, Atleta **atletiCognome, Atleta **atletiCategoria, int n);

// alloca lo spazio per il vettore di puntatori
// e li fa puntare all'array originale
void atletaAlloc(Atleta ***atleti, Atleta *original, int n);

// legge i dati da file e si interfaccia con l'utente
// mediante un menu basato su intero
int main(void)
{
    Atleta *atleti;
    Atleta **atletiData, **atletiID, **atletiCognome, **atletiCategoria;
    int n;      // numero di atleti letti
    int scelta; // scelta del menu
    int i;      // indice di ritorno delle ricerche

    n = leggi(&atleti);

    // alloco spazio per i vettori di puntatori
    atletaAlloc(&atletiData, atleti, n);
    atletaAlloc(&atletiID, atleti, n);
    atletaAlloc(&atletiCognome, atleti, n);
    atletaAlloc(&atletiCategoria, atleti, n);

    // inizializzo i vettori di puntatori con l'ordinamento giusto
    sort(atletiData, n, lessData);
    sort(atletiID, n, lessID);
    sort(atletiCognome, n, lessCognome);
    sort(atletiCategoria, n, lessCategoria);

    printf("Dati letti con successo.\n");
    printf("Scegliere opzione:\n");
    printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n");
    scanf("%d", &scelta);
    do
    {
        fflush(stdin);

        // lo stato viene aggiornato ogni volta che si richiama una
        // funzione che cambia l'ordinamento
        switch (scelta)
        {
            case 0: destroy(atleti, atletiData, atletiID, atletiCognome, atletiCategoria, n);  return 0;
            case 1: stampa(atleti, n);                                                         break;
            case 2: stampaCategoria(atletiCategoria, n);                                       break;
            case 3: aggiornaOre(atletiID, n);                                                  break;

            // passo n - 1 perché la ricerca è basata sugli indici l e r
            case 4: i = codeSearch(atletiID, n - 1);
                    if (i != -1) stampaAtleta(*atletiID[i], stdout);
                    break;
            case 5: i = nameSearch(atletiCognome, n - 1);
                    if (i != -1) stampaAtleta(*atletiCognome[i], stdout);
                    break;
					
			default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n");
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


void sort(Atleta **atleti, int n, int less(Atleta *, Atleta *))
{
    int i, j;
    Atleta *x;

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

int lessData(Atleta *a1, Atleta *a2)
{
    int dd, mm, yy;

    char *data1 = malloc((strlen(a1->data) + 1) * sizeof(*data1));
    char *data2 = malloc((strlen(a2->data) + 1) * sizeof(*data2));

    sscanf(a1->data, "%2d/%2d/%4d", &dd, &mm, &yy);
    sprintf(data1, "%4d/%02d/%02d", yy, mm, dd);

    sscanf(a2->data, "%2d/%2d/%4d", &dd, &mm, &yy);
    sprintf(data2, "%4d/%02d/%02d", yy, mm, dd);

    return strcmp(data1, data2) < 0;
}

int lessID(Atleta *a1, Atleta *a2)
{
    return strcmp(a1->id, a2->id) < 0;
}

int lessCognome(Atleta *a1, Atleta *a2)
{
    return strcmp(a1->cognome, a2->cognome) < 0;
}

int lessCategoria(Atleta *a1, Atleta *a2)
{
    return strcmp(a1->categoria, a2->categoria) < 0;
}

void stampaCategoria(Atleta **atleti, int n)
{
    char categoria[MAXSTR + 1];

    int i;

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

        stampaAtleta(*atleti[i], stdout);
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


int codeSearch(Atleta **atleti, int n)
{
    char code[MAXSTR + 1];
    int i, l;

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    l = 0;

    while (l <= n)
    {
        i = l + (n - l) / 2;

        if (strcmp(atleti[i]->id, code) == 0)
            return i;

        if (strcmp(atleti[i]->id, code) < 0)
            l = i + 1;
        else
            n = i - 1;
    }


    printf("Atleta non trovato!\n");
    return -1;
}


int nameSearch(Atleta **atleti, int n)
{
    char name[MAXSTR + 1];
    int i, l;

    printf("Inserire il nome dell' atleta: ");
    scanf("%s", name);

    l = 0;

    while (l <= n)
    {
        i = l + (n - l) / 2;

        if (strcmp(atleti[i]->cognome, name) == 0)
            return i;

        if (strcmp(atleti[i]->cognome, name) < 0)
            l = i + 1;
        else
            n = i - 1;
    }


    printf("Atleta non trovato!\n");
    return -1;
}


void aggiornaOre(Atleta **atleti, int n)
{
    int i = codeSearch(atleti, n);
    int ore;

    stampaAtleta(*atleti[i], stdout);

    printf("Inserire ore: ");
    scanf("%d", &ore);

    atleti[i]->ore = ore;
}


void destroy(Atleta *atleti, Atleta **atletiData, Atleta **atletiID, Atleta **atletiCognome, Atleta **atletiCategoria, int n)
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

    free(atletiData);
    free(atletiID);
    free(atletiCognome);
    free(atletiCategoria);
    free(atleti);
}


void atletaAlloc(Atleta ***atleti, Atleta *original, int n)
{
    int i;

    *atleti = malloc(n * sizeof(**atleti));
    if (*atleti == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    for (i = 0; i < n; i++)
        (*atleti)[i] = &original[i];
}
