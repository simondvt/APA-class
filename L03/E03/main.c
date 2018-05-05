#include <stdio.h>
#include <stdlib.h>

#define MAX 50
#define VISITED '='
#define WIN 1
#define FAIL 0

char mat[MAX][MAX];
int semplice = 1;   // flag booleana
int len = 1;        // lunghezza
int r, c;           // dimensioni reali


// controlla la casella [x][y]
void check(int x, int y);

// termina il programma gestendo il caso di vittoria / fallimento
void terminate(int fine);

int main(int argc, char *argv[])
{
    FILE *matF, *spostamentiF;
    int i, j;           // contatori for
    int curX, curY;     // posizione casella corrente
    int dx, dy;         // spostamenti

    if (argc < 2)
    {
        printf("Uso: %s <matrice> <spostamenti>\n", argv[0]);
        return 1;
    }

    if ((matF = fopen(argv[1], "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", argv[1]);
        return 1;
    }

    if ((spostamentiF = fopen(argv[2], "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", argv[2]);
        return 1;
    }

    fscanf(matF, "%d %d", &r, &c);

	
	// lettura dati e localizzazione della casella di entrata
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            fscanf(matF, " %c", &mat[i][j]);

            if (mat[i][j] == 'I')
            {
                curX = i;
                curY = j;
            }
        }
    }

    fclose(matF);


    while (fscanf(spostamentiF, "%d %d", &dx, &dy) != EOF)
    {
        if (dx * dy != 0)  // diagonale
        {
            // si suppone che dx sia in modulo uguale a dy

            if (dx > 0 && dy > 0)
            {
                for (i = 0; i < dx; i++)
                {
                    curY++;
                    curX--;
                    check(curX, curY);
                    len++;
                }
            }
            else if (dx < 0 && dy < 0)
            {
                for (i = 0; i > dx; i--)
                {
                    curY--;
                    curX++;
                    check(curX, curY);
                    len++;
                }
            }
            else if (dx > 0 && dy < 0)
            {
                for (i = 0; i < dx; i++)
                {
                    curY++;
                    curX++;
                    check(curX, curY);
                    len++;
                }
            }
            else if (dx < 0 && dy > 0)
            {
                for (i = 0; i > dx; i--)
                {
                    curY--;
                    curX--;
                    check(curX, curY);
                    len++;
                }
            }
        }
        else  // orizzontale / verticale
        {
            if (dx != 0)  // orizzontale
            {
                if (dx > 0)
                {
                    for (i = 0; i < dx; i++)
                    {
                        curY++;
                        check(curX, curY);
                        len++;
                    }
                }
                else
                {
                    for (i = 0; i > dx; i--)
                    {
                        curY--;
                        check(curX, curY);
                        len++;
                    }
                }
            }
            else  // verticale
            {
                if (dy > 0)
                {
                    for (i = 0; i < dy; i++)
                    {
                        curX--;
                        check(curX, curY);
                        len++;
                    }
                }
                else
                {
                    for (i = 0; i > dy; i--)
                    {
                        curX++;
                        check(curX, curY);
                        len++;
                    }
                }
            }
        }
    }

    fclose(spostamentiF);

    printf("Con gli spostamenti ricevuti NON sei arrivato all'uscita.\n");

    return 0;
}


void check(int x, int y)
{
    if (x < 0 || y < 0 || x >= r || y >= c)
    {
        printf("[ERR] Sono fuori dai bordi della matrice.\n");
        exit(1);
    }

    switch (mat[x][y])
    {
        case VISITED: semplice = 0;         break;
        case '-'    : mat[x][y] = VISITED;  break;
        case 'X'    : terminate(FAIL);      break;
        case 'U'    : terminate(WIN);       break;
    }
}


void terminate(int fine)
{
    if (fine == WIN)
        printf("Il cammino esiste di lunghezza %d %s e' semplice.\n", len,
               semplice ? "ed " : "ma NON ");

    else
        printf("Il cammino NON esiste.\n");


    exit(0);
}
