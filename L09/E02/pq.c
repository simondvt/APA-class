#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "PQ.h"

struct pqueue { Item *array; int heapsize; };

int LEFT(int i) {
  return i*2+1;
}

int RIGHT(int i) {
  return i*2+2;
}

int PARENT(int i) {
  return (i-1)/2;
}

PQ PQinit(int maxN){
  PQ pq;
  pq = malloc(sizeof(*pq));
  pq->array = (Item *)malloc(maxN*sizeof(Item));
  pq->heapsize = 0;
  return pq;
}

int PQempty(PQ pq) {
  return pq->heapsize == 0;
}

int PQsize(PQ pq) {
  return pq->heapsize;
}

void PQinsert (PQ pq, Item item) {
  int i;
  i  = pq->heapsize++;
  while( (i>=1) && (ITEMgreater(pq->array[PARENT(i)], item)) ) {
    pq->array[i] = pq->array[PARENT(i)];
    i = (i-1)/2;
  }
  pq->array[i] = item;
  return;
}

void Swap(PQ pq, int n1, int n2) {
  Item temp;

  temp  = pq->array[n1];
  pq->array[n1] = pq->array[n2];
  pq->array[n2] = temp;
  return;
}


void Heapify(PQ pq, int i) {
  int l, r, largest;
  l = LEFT(i);
  r = RIGHT(i);
  if ( (l < pq->heapsize) && (ITEMless(pq->array[l], pq->array[i])) )
    largest = l;
  else
    largest = i;
  if ( (r < pq->heapsize) && (ITEMless(pq->array[r], pq->array[largest])))
    largest = r;
  if (largest != i) {
    Swap(pq, i,largest);
	Heapify(pq, largest);
  }
  return;
}

Item PQextractMax(PQ pq) {
  Item item;
  Swap (pq, 0,pq->heapsize-1);
  item = pq->array[pq->heapsize-1];
  pq->heapsize--;
  Heapify(pq, 0);
  return item;
}

Item PQshowMax(PQ pq) {
  return pq->array[0];
}

void PQdisplay(PQ pq) {
  int i;
  for (i = 0; i < pq->heapsize; i++)
    ITEMshow(pq->array[i], stdout);
}

void PQchange (PQ pq, int pos, Item item) {
  while( (pos>=1) && (ITEMgreater(pq->array[PARENT(pos)], item)) ) {
    pq->array[pos] = pq->array[PARENT(pos)];
	pos = (pos-1)/2;
  }
  pq->array[pos] = item;
  Heapify(pq, pos);
  return;
}

void PQdelete(PQ pq)
{
    int i;
    char name[100];

    printf("Inserire nome partecipante: ");
    scanf("%s", name);

    for (i = 0; i < pq->heapsize; i++)
    {
        if (strcmp(name, getName(pq->array[i])) == 0)
        {
            // sposto l'emento da cancellare in fondo
            Swap (pq, i, pq->heapsize-1);
            pq->heapsize--;
            Heapify(pq, 0);

            return;
        }
    }

    printf("Nessun partecipante trovato\n");
}

void PQshow(PQ pq)
{
    Item item;

    if (pq->heapsize <= 0)
        return;

    item = PQextractMax(pq);
    ITEMshow(item, stdout);
    PQshow(pq);
    PQinsert(pq, item);
}



void gioca(PQ pq)
{
    Item i1, i2;

    i1 = PQextractMax(pq);
    i2 = PQextractMax(pq);

    printf("Giocano %s e %s\n", getName(i1), getName(i2));

    if (rand() < RAND_MAX / 2) // vince i1
    {
        setPoints(i1, getPoints(i1) + ceil(0.25 * getPoints(i2)));
        setPoints(i2, getPoints(i2) - ceil(0.25 * getPoints(i2)));

        printf("Ha vinto %s con %d punti\n\n", getName(i1), getPoints(i1));
        PQinsert(pq, i1);

        if (getPoints(i2) <= 0)
        {
            printf("%s perde!\n", getName(i2));
        }
        else
        {
            PQinsert(pq, i2);
        }
    }
    else // vince i2
    {
        setPoints(i2, getPoints(i2) + ceil(0.25 * getPoints(i1)));
        setPoints(i1, getPoints(i1) - ceil(0.25 * getPoints(i1)));

        printf("Ha vinto %s con %d punti\n\n", getName(i2), getPoints(i2));
        PQinsert(pq, i2);

        if (getPoints(i1) <= 0)
        {
            printf("%s perde!\n", getName(i1));
        }
        else
        {
            PQinsert(pq, i1);
        }
    }
}

void caricaFile(PQ pq)
{
    FILE *fp;
    Item item;
    int points;
    char buf[100];

    printf("Inserisci file: ");
    scanf("%s", buf);

    if ((fp = fopen(buf, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", buf);
        exit(EXIT_FAILURE);
    }

    while((item = ITEMscan(fp)) != NULL)
    {
        fscanf(fp, " %d\n", &points);
        setPoints(item, points);
        PQinsert(pq, item);
    }


    printf("File caricato\n");
    fclose(fp);
}

void salvaFile(PQ pq)
{
    Item item;
    int i;
    char buf[100];
    FILE *fp;

    printf("Inserisci file: ");
    scanf("%s", buf);

    if ((fp = fopen(buf, "w")) == NULL)
    {
        printf("Impossibile scrivere su %s\n", buf);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < pq->heapsize; i++)
    {
        ITEMshow(pq->array[i], fp);
    }

    fclose(fp);
}
