/****************************
* Trattasi del principio di *
* moltiplicazione           *
*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN "sets.txt"   // file di input
#define OUT "out.txt"   // file di output

#define MAX 10  // cardinalità massima

typedef struct
{
    int num_scelte;
    char *scelte;
} Livello;

// legge le scelte dal file "in" e le memorizza nel vettore val
int leggi(Livello **val, char *in);

// funzione che ricorsiva, salva ogni sigla nel file out
void princ_molt(int pos, Livello *val, char *sol, int n, FILE *outFile);

int main(void)
{
    FILE *outFile;
    Livello *val;
    int n, i;
    char *sol;

    n = leggi(&val, IN);
    outFile = fopen(OUT, "w");
    sol = malloc((n + 1) * sizeof(*sol));

    princ_molt(0, val, sol, n, outFile);


    // libero la memoria e chiudo il file
    for (i = 0; i < n; i++)
        free(val[i].scelte);
    free(val);
    free(sol);
    fclose(outFile);
    return 0;
}

int leggi(Livello **val, char *in)
{
    int i, n;
    char buf[MAX + 1];
    int len;
    FILE *inFile;

    if ((inFile = fopen(in, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", IN);
        exit(EXIT_FAILURE);
    }

    fscanf(inFile, "%d\n", &n);

    if (((*val) = malloc(n * sizeof(**val))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(inFile, "%s", buf);
        len = strlen(buf);
        (*val)[i].num_scelte = len;
        (*val)[i].scelte = malloc((len + 1) * sizeof(*((*val)[i].scelte)));
        strncpy((*val)[i].scelte, buf, len);
    }

    fclose(inFile);
    return n;
}


void princ_molt(int pos, Livello *val, char *sol, int n, FILE *outFile)
{
    int i;

    if (pos >= n)
    {
        // essendo "sol" una stringa devo preoccuparmi di terminarla
        sol[n] = '\0';
        fprintf(outFile, "%s\n", sol);

        return;
    }

    for (i = 0; i < val[pos].num_scelte; i++)
    {
        sol[pos] = val[pos].scelte[i];
        princ_molt(pos + 1, val, sol, n, outFile);
    }
}
