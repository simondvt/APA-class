#include <stdio.h>
#include <string.h>

int mat_search (char mat[20][20], int r, int c, char s[20]);

int main(void)
{
    int i, j;           // contatori for
    int r = 4, c = 5;   // dimensioni reali della matrice
    char s[] = "foo";

    char mat[20][20] =
    {
        {'x', 'f', 'o', 'o', 'x'},
        {'x', 'o', 'x', 'z', 'f'},
        {'y', 'o', '2', 'f', 'o'},
        {'g', '4', 'x', 'a', 'o'},
    };


    // stampo la matrice
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
            printf("%c", mat[i][j]);

        printf("\n");
    }

    printf("La stringa %s appare nella matrice %d volte.\n", s, mat_search(mat, r, c, s));

    return 0;
}

int mat_search (char mat[20][20], int r, int c, char s[20])
{
    int count = 0;      // numero di occorrenze della stringa
    int matched = 0;    // numero di caratteri della stringa che ho riscontrato finora; serve anche da indice
    int i, j;

    // controllo righe da sx a dx
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (s[matched] == mat[i][j])
                matched++;

            if (matched == strlen(s))
            {
                count++;
                matched = 0;
            }
        }

        matched = 0;
    }

    // controllo righe da dx a sx
    for (i = 0; i < r; i++)
    {
        for (j = c - 1; j >= 0; j--)
        {
            if (s[matched] == mat[i][j])
                matched++;

            if (matched == strlen(s))
            {
                count++;
                matched = 0;
            }
        }

        matched = 0;
    }

    // controllo righe da sopra a sotto
    for (j = 0; j < c; j++)
    {
        for (i = 0; i < r; i++)
        {
            if (s[matched] == mat[i][j])
                matched++;

            if (matched == strlen(s))
            {
                count++;
                matched = 0;
            }
        }

        matched = 0;
    }

    // controllo righe da sotto a sopra
    for (j = 0; j < c; j++)
    {
        for (i = r - 1; i >= 0; i--)
        {
            if (s[matched] == mat[i][j])
                matched++;

            if (matched == strlen(s))
            {
                count++;
                matched = 0;
            }
        }

        matched = 0;
    }

    return count;
}
