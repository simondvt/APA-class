#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "ST.h"
#include "PQ.h"

#define NIL -1
#define INF 99999

typedef struct edge
{
    int dest;
    int w;

    struct edge *next;
} *Edge;

typedef struct kedge
{
    int i1, i2;
    int w;
} Kedge;    // Kruskal edge

typedef struct
{
	int key;
	int pi;
} Vertex;

struct graph
{
    int V, E;           // numero nodi e archi
    ST st;              // tabella di hash
    Edge *adjList;      // vettore di lista adiacenze
    int **shortMatrix;  // matrice che contiene i pesi dei cammini minimi
    int *removed;       // vettore di flag
	Vertex *vertexes;	// vettore vertici (per Prim)
};

Graph GraphInit(int V)
{
    int i;
    Graph g = malloc(sizeof(*g));

    g->V = V;
    g->E = 0;
    g->st = STinit(V);
    g->adjList = calloc(V, sizeof(*g->adjList));
    g->shortMatrix = NULL;
    g->removed = malloc(g->V * sizeof(*g->removed));
	g->vertexes = malloc(g->V * sizeof(*g->vertexes));
    for (i = 0; i < g->V; i++)
        g->removed[i] = 0;

    return g;
}

void insertEdge(Graph g, int i1, int i2, int w)
{
	g->E++;
	// creco l'arco per i1
	Edge e = malloc(sizeof(*e));
	e->dest = i2;
	e->w = w;
	e->next = g->adjList[i1];

	// aggiorno la lista di archi
	g->adjList[i1] = e;

	// creco l'arco per i2
	e = malloc(sizeof(*e));
	e->dest = i1;
	e->w = w;
	e->next = g->adjList[i2];

	// aggiorno la lista di archi
	g->adjList[i2] = e;
}

void readAdjList(Graph g, FILE *fp)
{
    char buf[MAX_LEN + 1];
    char *e1, *e2;
    int i1, i2;
    int w;

    while (fscanf(fp, "%s", buf) != EOF)
    {
        e1 = strdup(buf);
        fscanf(fp, "%s", buf);
        e2 = strdup(buf);
        fscanf(fp, "%d", &w);

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

        insertEdge(g, i1, i2, w);
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

void insertVertex(Graph g, char *name)
{
    STinsert(g->st, name);
}

static void printMatrix(Graph g)
{
    int i, j;

    for (i = 0; i < g->V; i++)
    {
        for (j = i; j < g->V; j++)
        {
            if (g->shortMatrix[i][j] != INF && i != j)
			{
				printf("%s <-> %s\t%d\n", STkey(g->st, i), STkey(g->st, j), g->shortMatrix[i][j]);
			}
        }
    }
}

// Costruisce la matrice dei pesi g->wMatrix
static void generateWeigthMatrix(Graph g)
{
    int i, j;
    Edge cur;

    g->shortMatrix = malloc(g->V * sizeof(*g->shortMatrix));
    for (i = 0; i < g->V; i++)
    {
        g->shortMatrix[i] = malloc(g->V * sizeof(*g->shortMatrix[i]));
        for (j = 0; j < g->V; j++)
            if (i != j)
                g->shortMatrix[i][j] = INF;
            else
                g->shortMatrix[i][j] = 0;

        for (cur = g->adjList[i]; cur != NULL; cur = cur->next)
        {
            g->shortMatrix[i][cur->dest] = cur->w;
        }
    }
}

void FW(Graph g)
{
    int i, j, k;

    generateWeigthMatrix(g);

    for (k = 0; k < g->V; k++)
    {
        for (i = 0; i < g->V; i++)
        {
            for (j = 0; j < g->V; j++)
            {
                if (g->shortMatrix[i][k] + g->shortMatrix[k][j] < g->shortMatrix[i][j])
                    g->shortMatrix[i][j] = g->shortMatrix[i][k] + g->shortMatrix[k][j];
            }
        }
    }

    // g->shortMatrix[i][j] contiene la distanza minima tra i e j
    // stampo il risultato
    printMatrix(g);
}

static int isCyclicWrap(Graph g, int v, int *visited, int parent)
{
    Edge cur;

    visited[v] = 1;

    for (cur = g->adjList[v]; cur != NULL; cur = cur->next)
    {
        if (!g->removed[cur->dest])
        {
            if (!visited[cur->dest])
            {
                if (isCyclicWrap(g, cur->dest, visited, v))
                    return 1;
            }
            else if (cur->dest != parent)
                return 1;
        }
    }

    return 0;
}

int isCyclic(Graph g)
{
    int i;

    int *visited = malloc(g->V * sizeof(int));
    for (i = 0; i < g->V; i++)
        visited[i] = 0;

    for (i = 0; i < g->V; i++)
        if (!g->removed[i] && !visited[i])
            if (isCyclicWrap(g, i, visited, NIL))
                return 1;

    return 0;
}

void removeVertex(Graph g, int s)
{
    g->removed[s] = 1;
}

void addVertex(Graph g, int s)
{
	g->removed[s] = 0;
}

int getV(Graph g)
{
    return g->V;
}

char *getName(Graph g, int s)
{
    return STkey(g->st, s);
}

static void dfsCCR(Graph g, int v, int id, int *cc)
{
	Edge cur;

	cc[v] = id;

	for (cur = g->adjList[v]; cur != NULL; cur = cur->next)
		if (!g->removed[cur->dest] && cc[cur->dest] == -1)
			dfsCCR(g, cur->dest, id, cc);
}

int CC(Graph g, int *cc)
{
	int v;
	int id = 0;

	for (v = 0; v < g->V; v++)
		cc[v] = -1;

	for (v = 0; v < g->V; v++)
		if (!g->removed[v] && cc[v] == -1)
			dfsCCR(g, v, id++, cc);

	return id;
}

void removeEdge(Graph g, int i1, int i2)
{
	Edge cur, prev;

	g->E--;
	// cancello nella lista di i1
	for (cur = g->adjList[i1], prev = NULL; cur->dest != i2; prev = cur, cur = cur->next);
	if (prev == NULL)
	{
		g->adjList[i1] = cur->next;
		free(cur);
	}
	else
	{
		prev->next = cur->next;
		free(cur);
	}

	// cancello nella lista di i2
	for (cur = g->adjList[i2], prev = NULL; cur->dest != i1; prev = cur, cur = cur->next);
	if (prev == NULL)
	{
		g->adjList[i2] = cur->next;
		free(cur);
	}
	else
	{
		prev->next = cur->next;
		free(cur);
	}
}

int existVertex(Graph g, int s)
{
	return g->removed[s] == 0;
}

int dist(Graph g, int i1, int i2)
{
	return g->shortMatrix[i1][i2];
}

static int cmpEdge(const void *e1, const void *e2)
{
    return ((Kedge *) e1)->w - ((Kedge *) e2)->w;
}

static int find(int x, int *id)
{
    while (x != id[x])
        x = id[x];

    return x;
}

static int UFfind(int p, int q, int *id)
{
    return find(p, id) == find(q, id);
}

static void UFunion(int p, int q, int *id)
{
    int i = find(p, id);
    int j = find(q, id);

    if (i == j)
        return;

    id[i] = j;
}

int kruskal(Graph g)
{
    Kedge *e = malloc(g->E * sizeof(*e));
    int E = 0, V = 0;
    Edge cur;
    int i, k, *id;
    int total = 0;

    for (i = 0; i < g->V; i++)
    {
        if (existVertex(g, i))
        {
            V++;

            for (cur = g->adjList[i]; cur != NULL; cur = cur->next)
            {
                if (i < cur->dest)
                {
                    Kedge nuovo;
                    nuovo.i1 = i;
                    nuovo.i2 = cur->dest;
                    nuovo.w = cur->w;

                    e[E++] = nuovo;
                }
            }
        }
    }

    qsort(e, E, sizeof(*e), cmpEdge);

    id = malloc(g->V * sizeof(*id));
    for (i = 0;i < g->V; i++)
        id[i] = i;

    for (i = 0, k = 0; k < E && i < V - 1; i++)
    {
        if (!UFfind(e[i].i1, e[i].i2, id))
        {
            UFunion(e[i].i1, e[i].i2, id);
            total += e[i].w;
        }
    }

    return total;
}

void deleteGrafo(Graph g)
{
    int i;

    deleteST(g->st);
    free(g->removed);
    for (i = 0; i < g->V; i++)
    {
        free(g->adjList[i]);
        free(g->shortMatrix[i]);
    }

    free(g->adjList);
    free(g->vertexes);
    free(g->shortMatrix);
    free(g);
}