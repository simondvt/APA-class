#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"

Graph read(FILE *fp);

int main(int argc, char *argv[])
{
    char v[MAX_LEN + 1];
    Graph g;
    FILE *fp;
    int scelta;

    if (argc != 2)
    {
        printf("Uso: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    g = read(fp);
    fclose(fp);

    printf("0 - Esci\n1 - Stampa vertici\n2 - Stampa archi di un vertice\n3 - Stampa matrice di adiacenza\n4 - Flusso di rete: ");
    scanf("%d", &scelta);

    for(;;)
    {
        switch (scelta)
        {
            case 0: return 0;                   break;
            case 1: printVertex(g);             break;
            case 2: printf("Nome vertice: ");
                    scanf("%s", v);
                    printEdges(g, v);           break;
            case 3: generateMatrix(g);          break;
            case 4: flux(g);                    break;

            default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa vertici\n2 - Stampa archi di un vertice\n3 - Stampa matrice di adiacenza\n4 - Flusso di rete: ");
        scanf("%d", &scelta);
    }

//    printf("Vertici:\n");
//    printVertex(g);
//    printf("\nArchi:\n");
//    printEdges(g, "Vivi");
//    generateMatrix(g);
    flux(g);

    return 0;
}

Graph read(FILE *fp)
{
    Graph g;
    int i, V;

    fscanf(fp, "%d", &V);
    g = GraphInit(V);

    for (i = 0; i < V; i++)
        insertVertex(g, fp);

    readAdjList(g, fp);

    return g;
}