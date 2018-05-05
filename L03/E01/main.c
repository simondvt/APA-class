#include <stdio.h>

#define MAX 50
#define MAT "mat.txt"

#define VISITED (-1)

int mat[MAX][MAX];


// trovata una prima riga di caselle nere, scendo lungo la y
// per vedere dove finisce la regione
void searchMatrix(int topX, int topY, int endX, int endY);

int main(void)
{
    int i, j;       // contatori for
    int r, c;       // dimensioni reali
    int topX = -1, topY = -1; // estremo superiore sinistro
    FILE *fp;

    if ((fp = fopen(MAT, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", MAT);
        return 1;
    }

    // acquisizione matrice
    fscanf(fp, "%d %d", &r, &c);
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            fscanf(fp, " %d", &mat[i][j]);

    fclose(fp);


    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (mat[i][j] == 1 && topX < 0 && topY < 0) // se sono all'inizio della regione
            {
                topX = i;
                topY = j;
            }

            if (mat[i][j] != 1 && topX >= 0 && topY >= 0) // sono uscito dalla regione
            {
                searchMatrix(topX, topY, i, j);
                topX = topY = -1;
            }

            mat[i][j] = VISITED;
        }


		// Gestisce il caso al bordo:
		// se sono ancora in una regione (topX e topY non negativi) ma sono
		// arrivato all'ultima colonna, non troverò un mat[i][j] != 1,
		// quindi manualmente faccio controllare la sotto-matrice
        if (topX >= 0 && topY >= 0)
        {
            searchMatrix(topX, topY, i, j);
            topX = topY = -1;
        }
    }

    return 0;
}

void searchMatrix(int topX, int topY, int endX, int endY)
{
    static int count = 0; // tengo traccia del numero di regioni

    int i, j;
    int fine = 0;
    int base = endY - topY;
    int altezza = 0;


    for (i = topX; !fine; i++)
    {
        for (j = topY; j < endY && !fine; j++)
        {
            if (mat[i][j] == 0)
                fine = 1;
            else
                mat[i][j] = VISITED;
        }

        if (!fine)
            altezza++;
    }

    count++;

    printf("Regione %d: estr. sup. SX = <%d, %d> b = %d, h = %d, Area = %d\n",
           count ,topX, topY, base, altezza, base * altezza);
}
