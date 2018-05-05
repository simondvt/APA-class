#include <stdlib.h>
#include <limits.h>
#include "graph.h"
#include "queue.h"

typedef enum {WHITE, GRAY, BLACK} Color;

typedef struct
{
    Color color;
    int dist;
    int pi;
} Vertex;

typedef struct edge
{
    int dest;

    struct edge *next;
} *Edge;

struct graph
{
    int V, E;           // numero nodi e archi
    Edge *adjList;      // vettore di lista adiacenze
    Vertex *vertexes;   // vettore vertici
};

Graph GraphInit(int V)
{
    Graph g = malloc(sizeof(*g));

    g->V = V;
    g->E = 0;
    g->adjList = calloc(V, sizeof(*g->adjList));
    g->vertexes = malloc(V * sizeof(*g->vertexes));

    return g;
}

void insertEdge(Graph g, int i1, int i2)
{
    g->E++;
    // creco l'arco per i1
    Edge e = malloc(sizeof(*e));
    e->dest = i2;
    e->next = g->adjList[i1];

    // aggiorno la lista di archi
    g->adjList[i1] = e;

    // creco l'arco per i2
    e = malloc(sizeof(*e));
    e->dest = i1;
    e->next = g->adjList[i2];

    // aggiorno la lista di archi
    g->adjList[i2] = e;
}

void BFS(Graph g, int s)
{
    int i;
    int u;
    Queue q = QInit();
    Edge v;

    for (i = 0; i < g->V; i++)
    {
        if (i != s)
        {
            g->vertexes[i].color = WHITE;
            g->vertexes[i].dist = INT_MAX;
            g->vertexes[i].pi = -1;
        }
    }

    g->vertexes[s].color = GRAY;
    g->vertexes[s].dist = 0;
    g->vertexes[s].pi = -1;

    enqueue(q, s);
    while(!isEmpty(q))
    {
        u = dequeue(q);

        for (v = g->adjList[u]; v != NULL; v = v->next)
        {
            if (g->vertexes[v->dest].color == WHITE)
            {
                g->vertexes[v->dest].color = GRAY;
                g->vertexes[v->dest].dist = g->vertexes[u].dist + 1;
                g->vertexes[v->dest].pi = u;
                enqueue(q, v->dest);
            }
        }

        g->vertexes[u].color = BLACK;
    }
}

int getDist(Graph g, int v)
{
    return g->vertexes[v].dist;
}

void findPath(Graph g, int v, int nc, char **mat)
{
    if (g->vertexes[v].pi == -1)
        return;

    findPath(g, g->vertexes[v].pi, nc, mat);
    mat[v / nc][v % nc] = 'O';
}