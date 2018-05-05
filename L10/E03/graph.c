#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "ST.h"

typedef struct edge
{
    int dest;
    int w;

    struct edge *next;
} *Edge;

typedef struct net
{
    int v;
    char *network;
} Net;

struct graph
{
    int V, E;           // numero nodi e archi
    ST st;              // tabella di hash
    Edge *adjList;      // vettore di lista adiacenze
    int **adjMatrix;    // matrice delle adiacenze
    Net *Web;           // vettore delle sotto-reti
};

Graph GraphInit(int V)
{
    Graph g = malloc(sizeof(*g));

    g->V = V;
    g->E = 0;
    g->st = STinit(V);
    g->adjList = calloc(V, sizeof(*g->adjList));
    g->adjMatrix = NULL;
    g->Web = malloc(V * sizeof(*g->Web));

    return g;
}

void readAdjList(Graph g, FILE *fp)
{
    char buf[MAX_LEN + 1];
    char *e1, *e2;
    int i1, i2;
    int flusso;

    while (fscanf(fp, "%s", buf) != EOF)
    {
        e1 = strdup(buf);
        fscanf(fp, "%s", buf);
        e2 = strdup(buf);
        fscanf(fp, "%d", &flusso);

        // cerco gli indici corrispondendi ai nomi dei vertici
        // se non li trovo li inserisco nella tabella
        i1 = STsearch(g->st, e1);
        if (i1 == -1)
        {
            printf("%s NON e' mai stato inserito.\n", e1);
            exit(EXIT_FAILURE);
        }
        i2 = STsearch(g->st, e2);
        if (i2 == -1)
        {
            printf("%s NON e' mai stato inserito.\n", e2);
            exit(EXIT_FAILURE);
        }

        g->E++;
        // creco l'arco per i1
        Edge e = malloc(sizeof(*e));
        e->dest = i2;
        e->w = flusso;
        e->next = g->adjList[i1];

        // aggiorno la lista di archi
        g->adjList[i1] = e;

        // creco l'arco per i2
        e = malloc(sizeof(*e));
        e->dest = i1;
        e->w = flusso;
        e->next = g->adjList[i2];

        // aggiorno la lista di archi
        g->adjList[i2] = e;
    }
}

void printVertex(Graph g)
{
    printf("Numero totale di vertici: %d\n", STcount(g->st));
    STdisplay(g->st);
}

void printEdges(Graph g, char *v)
{
    int i, count = 0;
    Edge cur;

    i = STsearch(g->st, v);
    for (cur = g->adjList[i]; cur != NULL; cur = cur->next)
    {
        count++;
        printf("%s\n", STkey(g->st, cur->dest));
    }

    printf("Totale archi: %d\n", count);
}

void insertVertex(Graph g, FILE *fp)
{
    char buf[MAX_LEN + 1];
    Net n;

    fscanf(fp, "%s", buf);
    n.v = STinsert(g->st, strdup(buf));

    fscanf(fp, "%s", buf);
    n.network = strdup(buf);

    g->Web[n.v] = n;
}

void generateMatrix(Graph g)
{
    int i, j;
    Edge cur;

    g->adjMatrix = malloc(g->V * sizeof(*g->adjMatrix));
    for (i = 0; i < g->V; i++)
    {
        g->adjMatrix[i] = calloc(g->V, sizeof(*g->adjMatrix[i]));

        for (cur = g->adjList[i]; cur != NULL; cur = cur->next)
        {
            g->adjMatrix[i][cur->dest] = cur->w;
        }
    }

    printf("Generata la matrice di adiacenze:\n");
    for (i = 0; i < g->V; i++)
    {
        for (j = 0; j < g->V; j++)
            printf("%3d ", g->adjMatrix[i][j]);

        printf("\n");
    }
}

void flux(Graph g)
{
    int i;
    Edge cur;
    int intra = 0, inter = 0;

    for (i = 0; i < g->V; i++)
    {
        for (cur = g->adjList[i]; cur != NULL; cur = cur->next)
        {
            if (strcmp(g->Web[i].network, g->Web[cur->dest].network) == 0)
                intra += cur->w;
            else
                inter += cur->w;
        }
    }

    printf("Intra = %d\nInter = %d\n", intra / 2, inter / 2);
}