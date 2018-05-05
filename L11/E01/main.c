#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"

typedef struct edge
{
	int i1, i2, w;
	struct edge *next;
} Edge;

// struttura che contiene la lista di archi e il peso del mst di una soluzione
typedef struct
{
	Edge *head;	// lista archi
	int totalW;	// peso MST
} Archi;

// struttura per il principio di moltiplicazione
typedef struct
{
	int num_scelte;
	int *scelte;
} Livello;

Graph read(FILE *fp);
void powersetVertex(Graph g, int k, int *sol);
void powerset_rVertex(Graph g, int k, int *sol, int n, int pos, int start, int *found);
void connect(Graph g);
Edge* insArchi(Archi *sol, int i1, int i2, int w);
Edge* delArchi(Archi *sol);
void copybest(Archi **sol, Archi **best);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <file_grafo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Impossibile aprire %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    Graph g = read(fp);
    fclose(fp);

    printf("Distanze minime: \n");
    FW(g);
    int *sol = malloc(getV(g) * sizeof(int));
    powersetVertex(g, getV(g), sol);
	free(sol);

	deleteGrafo(g);
    return 0;
}

Graph read(FILE *fp)
{
    Graph g;
    int i, V;
    char buf[MAX_LEN + 1];

    fscanf(fp, "%d", &V);
    g = GraphInit(V);

    for (i = 0; i < V; i++)
    {
        fscanf(fp, "%s", buf);
        insertVertex(g, strdup(buf));
    }

    readAdjList(g, fp);

    return g;
}

void powerset_rVertex(Graph g, int k, int *sol, int n, int pos, int start, int *found)
{
	int i;

	if (pos >= n)
	{
        if (!isCyclic(g))
        {
            printf("No cycle removing:\n");
            for (i = 0; i < pos; i++)
                printf("%s\n", getName(g, sol[i]));

			*found = n;
			connect(g);
        }

		return;
	}

	for (i = start; i < k; i++)
	{
		sol[pos] = i;
		removeVertex(g, i);

		powerset_rVertex(g, k, sol, n, pos + 1, i + 1, found);

		// backtrack
		addVertex(g, i);
	}
}

void powersetVertex(Graph g, int k, int *sol)
{
	int n;
	int found = 0;

	// gestione insieme vuoto
	if (!isCyclic(g))
	{
		printf("Graph acyclic\n");
		return;
	}

	for (n = 1; n <= k && (!found || n <= found); n++)
		powerset_rVertex(g, k, sol, n, 0, 0, &found);
}

int princ_moltEdge(Graph g, int *help, int pos, Livello *val, int n, Archi *best, Archi *sol, int count)
{
	int i;
	int d;

	if (pos >= n)
	{
		// arrivato qui, so che il grafo è connesso e aciclico
		// perché ho scelto archi tra tutte le componenti connesse
		sol->totalW = kruskal(g);
		if (sol->totalW < best->totalW)
		{
			// trovata nuova soluzione ottima
            copybest(&sol, &best);
		}

		return count + 1;
	}

	for (i = 0; i < val[pos].num_scelte; i++)
	{
	    help[pos] = val[pos].scelte[i];

		if (pos > 0)
		{
		    d = dist(g, help[pos - 1], help[pos]);
			insertEdge(g, help[pos - 1], help[pos], d);
            sol->head = insArchi(sol, help[pos - 1], help[pos], d);
		}

		princ_moltEdge(g, help, pos + 1, val, n, best, sol, count + 1);

		if (pos > 0)
        {
            // backtrack
            removeEdge(g, help[pos - 1], help[pos]);
            sol->head = delArchi(sol);
        }
	}

	return count;
}

void connect(Graph g)
{
    int V = getV(g);
	Livello *val = malloc(V * sizeof(*val));
	int *cc, *help;
	int i;
	Archi best, sol;
	best.totalW = INT_MAX;
	best.head = NULL;
	sol.head = NULL;
	cc = malloc(V * sizeof(*cc));
	int components = CC(g, cc);

	if (components == 1)
	{
		printf("CONNECTED\n");
		return;
	}

	for (i = 0; i < V; i++)
	{
		val[i].scelte = malloc(V * sizeof(*val[i].scelte));
		val[i].num_scelte = 0;
	}

	// preparo il vettore val
	for (i = 0; i < V; i++)
		if (existVertex(g, i))
            val[cc[i]].scelte[val[cc[i]].num_scelte++] = i;

    help = malloc(components * sizeof(*help));

    princ_moltEdge(g, help, 0, val, components, &best, &sol, 0);

    free(help);
    free(cc);
    printf("DISCONNECTED\n");
    printf("Add edges for a MST of weigth %d:\n", best.totalW);
    Edge *cur;
    for (cur = best.head; cur != NULL; cur = cur->next)
        printf("%s <-> %s w = %d\n", getName(g, cur->i1), getName(g, cur->i2), cur->w);
}

Edge* insArchi(Archi *sol, int i1, int i2, int w)
{
    Edge *cur = malloc(sizeof(*cur));
    cur->i1 = i1;
    cur->i2 = i2;
    cur->w = w;
    cur->next = sol->head;

    return cur;
}

Edge* delArchi(Archi *sol)
{
    Edge *cur;
    cur = sol->head;
    sol->head = sol->head->next;
    free(cur);

    return sol->head;
}

void copybest(Archi **sol, Archi **best)
{
    Edge *cur;
    (*best)->head = NULL;
    (*best)->totalW = (*sol)->totalW;

    for (cur = (*sol)->head; cur != NULL; cur = cur->next)
        (*best)->head = insArchi(*best, cur->i1, cur->i2, cur->w);
}