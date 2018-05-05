#include <stdio.h>
#include <stdlib.h>  // per la funzione exit()

#define MAX 50
#define MAT "mat.txt"

#define VISITED (-1)

// legge la matrice e salva by reference le sue dimensioni
void leggiMatrice(int M[MAX][MAX], int n, int *rows, int *cols);


// data una casella [r][c], scansione la matrice sottostante alla
// ricerca di una regione. Ritorna 1 se l'ha trovata, 0 altrimenti
int riconosciRegione(int M[MAX][MAX], int nr, int nc, int r, int c, int *b, int *h);

int main(void)
{
    int i, j;       // contatori for
    int r, c;       // dimensioni reali
    int count = 0;  // contatore regioni
    int base, altezza;
    int mat[MAX][MAX];


    leggiMatrice(mat, MAX, &r, &c);

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (riconosciRegione(mat, r, c, i, j, &base, &altezza))
                printf("Regione %d: estr. sup. SX = <%d, %d> b = %d, h = %d, Area = %d\n",
                       ++count, i, j, base, altezza, base * altezza);
        }
    }

    return 0;
}


void leggiMatrice(int M[MAX][MAX], int n, int *rows, int *cols)
{
    FILE *fp;
    int i, j;

    if ((fp = fopen(MAT, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", MAT);
        exit(1);
    }

    // acquisizione matrice
    fscanf(fp, "%d %d", rows, cols);
    for (i = 0; i < *rows; i++)
        for (j = 0; j < *cols; j++)
            fscanf(fp, " %d", &M[i][j]);

    fclose(fp);
}


int riconosciRegione(int M[MAX][MAX], int nr, int nc, int r, int c, int *b, int *h)
{
    int i, j;
    int found = 0, cFound;
    *b = 0;
    *h = 0;

    // se sono già in una regione ritorno 0
    if ((c > 0 && M[r][c - 1] == 1) || (r > 0 && M[r - 1][c] == 1))
        return 0;

    for (; r < nr; r++)
    {
        // se ho trovato l'inizio di una regione, controllo la sottomatrice sotto
        if (found)
        {
            for (c = cFound; c < *b + cFound; c++)
            {
                if (M[r][c] != 1)
                    return 1;
            }

            (*h)++;
        }

        else  // controllo se la casella [r][c] è l'inizio di una regione
        for (; c < nc && !found; c++)
        {
            if (M[r][c] == 1)
            {
                found = 1;
                cFound = c;
                (*b)++;
                (*h)++;

                // trovo la base
                while (c < nc && M[r][++c] == 1)
                    (*b)++;
            }
            else
                return 0;
        }
    }

    return 1;
}
