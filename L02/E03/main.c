#include <stdio.h>

#define MAXRIGA     20
#define MAXCOLONNA  20
#define SRC "mat.txt"

int mat[MAXRIGA][MAXCOLONNA];

// calcola la somma degli elementi della sotto matrice di estremi (startX, endX) e (startY, endY)
int somma(int startX, int endX, int startY, int endY);

int main(void)
{
    FILE *src;
    int row, col; // righe e colonne reali
    int r, c;     // indici inseriti dall utente
    int i, j;
    int NO, NE, SO, SE;

    if ((src = fopen(SRC, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", SRC);
        return 1;
    }


    // leggo e stampo la matrice
    fscanf(src, "%d %d", &row, &col);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            fscanf(src, " %d", &mat[i][j]);
            printf("%d ", mat[i][j]);
        }

        printf("\n");
    }

    fclose(src);

    printf("\nInserire indice di riga e colonna: ");
    scanf("%d%d", &r, &c);

    while (r >= 0 && c >= 0 && r < row && c < col)
    {
        NO = somma(0, r, 0, c);
        NE = somma(0, r, c + 1, col);
        SO = somma(r + 1, row, 0, c);
        SE = somma(r + 1, row, c + 1, col);

        printf("NO = %d\n", NO);
        printf("NE = %d\n", NE);
        printf("SO = %d\n", SO);
        printf("SE = %d\n", SE);

        printf("\nInserire indice di riga e colonna: ");
        scanf("%d%d", &r, &c);
    }

    return 0;
}


int somma(int startX, int endX, int startY, int endY)
{
    int sum = 0;
    int i, j;

    for (i = startX; i < endX; i++)
        for (j = startY; j < endY; j++)
            sum += mat[i][j];

    return sum;
}
