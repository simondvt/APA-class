#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>

#define MAX_LEN 30

typedef struct graph *Graph;

Graph GraphInit(int V);
void readAdjList(Graph g, FILE *fp);
void printVertex(Graph g);
void printEdges(Graph g, char *v);
void generateMatrix(Graph g);
void insertVertex(Graph g, char *name);
void FW(Graph g); // algoritmo di Floyd Warshall
int CC(Graph g, int *cc);  // ritorna il numero di componenti connesse
int kruskal(Graph g);	// ritorna il peso del MST calcolato con Kruskal
int isCyclic(Graph g);  // ritorna valore booleano true se il grafo è ciclico
void removeVertex(Graph g, int s);
void addVertex(Graph g, int s);
void insertEdge(Graph g, int i1, int i2, int w);
void removeEdge(Graph g, int i1, int i2);
void deleteGrafo(Graph g);

// getter
int getV(Graph g);
char *getName(Graph g, int s);
int existVertex(Graph g, int s);
int dist(Graph g, int i1, int i2);

#endif // GRAPH_H_INCLUDED
