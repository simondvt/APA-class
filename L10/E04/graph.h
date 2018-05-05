#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>

typedef struct graph *Graph;

Graph GraphInit(int V);
void insertEdge(Graph g, int i1, int i2);
void BFS(Graph g, int s);
int getDist(Graph g, int v);
void findPath(Graph g, int v, int nc, char **mat);

#endif // GRAPH_H_INCLUDED
