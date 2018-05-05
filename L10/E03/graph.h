#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>

#define MAX_LEN 30

typedef struct graph *Graph;

Graph GraphInit(int V);
void insertVertex(Graph g, FILE *fp);
void readAdjList(Graph g, FILE *fp);
void printVertex(Graph g);
void printEdges(Graph g, char *v);
void generateMatrix(Graph g);
void flux(Graph g);

#endif // GRAPH_H_INCLUDED
