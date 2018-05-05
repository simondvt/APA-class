#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SRCFILE "sequenze.txt"
#define MAXR 60
#define MAXC 80
#define MAXP 1000

// legge le righe dal source file in pagina[][]
// ritorna il numero effettivo di righe lette
int leggiPagina(char pagina[][MAXC + 1], int maxR);

// legge le parole dalla matrice pagina e le salva nell
// array di puntatori parole
int riconosciParole(char pagina[][MAXC + 1], int rows, char *parole[], int maxP);

// ritorna <0, 0, >0 a seconda che p1 sia minore, uguale, o maggiore di p2
// in senso alfabetico. Uso di spaceStrLen
int confrontaParole(char *p1, char *p2);

// versione modificata di strlen che continua a contare
// finché trova un carattere alfanumerico, ad indicare che
// la parola non è ancora finita
int alphaStrLen(char *p);

// effettua l' insertion sort su un array di puntatori a char
void insertionSort(char *parole[], int np);

// funzione di ricerca dicotomica
int ricercaBinaria(char *parole[], int np, char *cerca);


int main(void)
{
    char pagina[MAXR][MAXC + 1] = {{ '\0' }};
    char *parole[MAXP] = {NULL};
    int nr, np;
    char cerca[MAXC];   // sovralloco, una parola è lunga al massimo quanto una riga
    int binIndex;       // indice ritornato della ricerca binaria
    int lIndex, rIndex; // indici per la ricerca di altre occorrenza a destra e a sinitra

    nr = leggiPagina(pagina, MAXR);
    np = riconosciParole(pagina, nr, parole, MAXP);

    // ho np parole, con indice da 0 a np - 1
    // decremento np per semplicità di utilizzo
    np--;

    insertionSort(parole, np);

    printf("Inserisci parola: ");
    scanf("%s", cerca);

    while (strcmp(cerca, "$fine") != 0)
    {
        lIndex = rIndex = binIndex = ricercaBinaria(parole, np, cerca);

        if (binIndex >= 0)
        {
            printf("Parola \"%s\" trovata in (%d, %d)\n", cerca,
               (parole[binIndex] - pagina[0]) / (MAXC + 1), (parole[binIndex] - pagina[0]) % (MAXC + 1));

            // cerco a sinistra
            while (--lIndex >= 0 && confrontaParole(parole[lIndex], cerca) == 0)
                printf("Parola \"%s\" trovata in (%d, %d)\n", cerca,
                    (parole[lIndex] - pagina[0]) / (MAXC + 1), (parole[lIndex] - pagina[0]) % (MAXC + 1));

            // cerco a destra
            while (++rIndex <= np && confrontaParole(parole[rIndex], cerca) == 0)
                printf("Parola \"%s\" trovata in (%d, %d)\n", cerca,
                    (parole[rIndex] - pagina[0]) / (MAXC + 1), (parole[rIndex] - pagina[0]) % (MAXC + 1));
        }
        else
            printf("Parola non trovata!\n");

        printf("Inserisci parola: ");
        scanf("%s", cerca);
    }

    return 0;
}

int leggiPagina(char pagina[][MAXC + 1], int maxR)
{
    int nr = 0;
    FILE *src;

    if ((src = fopen(SRCFILE, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", SRCFILE);
        exit(1);
    }


    while (nr < maxR && fgets(pagina[nr], MAXC, src) != NULL)
        nr++;

    fclose(src);
    return nr;
}


int riconosciParole(char pagina[][MAXC + 1], int rows, char *parole[], int maxP)
{
    int np = 0;     // numero parole
    int i, j;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < MAXC && pagina[i][j] != '\n'; j++)
        {
            // se non ho salvato ancora nulla in parole[np],
            // e ho trovato un primo carattere valido per iniziare una parola
            if (parole[np] == NULL && isalpha(pagina[i][j]))
                parole[np] = &pagina[i][j];

            // se ho già agganciato una parola e ho trovato
            // un carattere che non può far parte di quella parola,
            // allora la parola è finita
            if (parole[np] != NULL && !isalpha(pagina[i][j]))
                np++;
        }

        // parole alla fine della riga
        if (parole[np] != NULL)
            np++;
    }

    return np;
}


int confrontaParole(char *p1, char *p2)
{
    int n1, n2;

    n1 = alphaStrLen(p1);
    n2 = alphaStrLen(p2);


    // quando si confronta "$fine", il primo carattere
    // essendo non alpha risulta di lunghezza pari a 0,
    // la strnicmp in questo caso ritorna 0, ma non è ciò che voglio
    if (n1 == 0)        return 1;
    else if (n2 == 0)   return -1;

    return strnicmp(p1, p2, n1 < n2 ? n1 : n2);
}


int alphaStrLen(char *p)
{
    int count = 0;

    while (isalpha(p[count]) && p[count] != '\0')
        count++;

    return count;
}


void insertionSort(char *parole[], int np)
{
    int i, j;
    char *temp;

	for (i = 1; i <= np; i++)
    {
        temp = parole[i];
        j = i - 1;

        while (j >= 0 && confrontaParole(temp, parole[j]) < 0)
        {
            parole[j + 1] = parole[j];
            j--;
        }

        parole[j + 1] = temp;
    }
}


int ricercaBinaria(char *parole[], int np, char *cerca)
{
    int i;
    int l = 0;

    while (l <= np)
    {
        i = l + (np - l) / 2;

        if (confrontaParole(parole[i], cerca) == 0)
            return i;

        if (confrontaParole(parole[i], cerca) < 0)
            l = i + 1;
        else
            np = i - 1;
    }

    return -1;
}
