#ifndef PQ_H_INCLUDED
#define PQ_H_INCLUDED

typedef struct pqueue *PQ;

PQ PQinit(int);
int PQempty(PQ);
void PQinsert(PQ, int);
int PQmin(PQ);
int inPQ(PQ, int);
void delPQ(PQ pq);

#endif // PQ_H_INCLUDED