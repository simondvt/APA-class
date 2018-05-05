#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

typedef struct node
{
    int v;
    struct node *next;
} *Node;

Graph matrix2graph(FILE *fp, int *start, Node *quit, int nr, int nc, char **mat);

int main(int argc, char *argv[])
{
    Graph g;
    FILE *fp;
    Node quit, cur, min;
    int start, i, j;
    int nr, nc;
    char **mat;

    if (argc != 2)
    {
        printf("Uso: %s <maze>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");

    fscanf(fp, "%d %d", &nr, &nc);
    mat = malloc(nr * sizeof(*mat));
    for (i = 0; i < nr; i++)
        mat[i] = malloc(nc * sizeof(*mat[i]));
    // genero un grafo a partire dalla matrice
    g = matrix2graph(fp, &start, &quit, nr, nc, mat);

    fclose(fp);

    printf("Letto il labirinto:\n");
    for (i = 0; i < nr; i++)
    {
        for (j = 0; j < nc; j++)
            printf("%c ", mat[i][j]);

        printf("\n");
    }

    // applico la ricerca in ampiezza al grafo
    BFS(g, start);

    // tra le tante uscite, trovo quella più breve
    min = quit;
    for (cur = quit->next; cur != NULL; cur = cur->next)
        if (getDist(g, cur->v) < getDist(g, min->v))
            min = cur;

    // stampo il percorso sulla matrice
    findPath(g, min->v, nc, mat);
    // stampo la matrice
    printf("\nUscita piu' vicina: %d blocchi\n", getDist(g, min->v));
    for (i = 0; i < nr; i++)
    {
        for (j = 0; j < nc; j++)
            printf("%c ", mat[i][j]);

        printf("\n");
    }

    return 0;
}

static int isSafe(int r, int c, int nr, int nc)
{
    return r >= 0 && c >= 0 && r < nr && c < nc;
}

Graph matrix2graph(FILE *fp, int *start, Node *quit, int nr, int nc, char **mat)
{
    Node n;
    Graph g;
    int i, j;
    char c;
    int pos = 0;

    g = GraphInit(nr * nc);
//    *quit = malloc(sizeof(*quit));
//    (*quit)->next = NULL;
    *quit = NULL;

    for (i = 0; i < nr; i++)
    {
        for (j = 0; j < nc; j++)
        {
            fscanf(fp, " %c", &c);
            mat[i][j] = c;

            if (c != 'X')
            {
                if (c == 'I')
                    *start = pos;
                if (c == 'U')
                {
                    n = malloc(sizeof(*n));
                    n->v = pos;
                    n->next = *quit;
                    *quit = n;
                }

                if (isSafe(i - 1, j, nr, nc) && mat[i - 1][j] != 'X')
                    insertEdge(g, pos, pos - nc);
                if (isSafe(i, j - 1, nr, nc) && mat[i][j - 1] != 'X')
                    insertEdge(g, pos, pos - 1);
            }

            pos++;
        }
    }

    return g;
}
