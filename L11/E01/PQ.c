#include <stdlib.h>
#include "PQ.h"

#define LEFT(i) 	2 * (i) + 1
#define RIGHT(i) 	2 * (i) + 2
#define PARENT(i)	((i) - 1) / 2

struct pqueue
{
	int *array;
	int heapsize;
};

PQ PQinit(int max)
{
	PQ pq = malloc(sizeof(*pq));
	pq->array = malloc(max * sizeof(*pq->array));
	pq->heapsize = 0;
	return pq;
}

int PQempty(PQ pq)
{
	return pq->heapsize == 0;
}

void PQinsert(PQ pq, int val)
{
	int i = pq->heapsize++;

	while (i >= 1 && val < pq->array[PARENT(i)])
	{
		pq->array[i] = pq->array[PARENT(i)];
		i = PARENT(i);
	}

	pq->array[i] = val;
}

static void swap(PQ pq, int n1, int n2)
{
	int temp;

	temp = pq->array[n1];
	pq->array[n1] = pq->array[n2];
	pq->array[n2] = temp;
}

static void heapify(PQ pq, int i)
{
	int l, r, lowest;

	l = LEFT(i);
	r = RIGHT(i);

	if (l < pq->heapsize && pq->array[l] < pq->array[i])
		lowest = l;
	else
		lowest = i;

	if (r < pq->heapsize && pq->array[r] < pq->array[lowest])
		lowest = r;

	if (lowest != i)
	{
		swap(pq, i, lowest);
		heapify(pq, lowest);
	}
}

int PQmin(PQ pq)
{
	int val;

	swap(pq, 0, pq->heapsize - 1);
	val = pq->array[pq->heapsize - 1];
	pq->heapsize--;
	heapify(pq, 0);
	return val;
}

int inPQ(PQ pq, int val)
{
	int i;

	for (i = 0; i < pq->heapsize; i++)
		if (pq->array[i] == val)
			return 1;

	return 0;
}

void delPQ(PQ pq)
{
    free(pq->array);
    free(pq);
}