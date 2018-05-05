#ifndef _DATO_INCLUDED
#define _DATO_INCLUDED

#include <stdio.h>
#define MAXC 100

typedef struct item* Item;
typedef int Key;

Item ITEMscan(FILE *fp);
Item ITEMrand();
Item ITEMsetvoid();
void ITEMshow(Item data, FILE *fp);
int ITEMless(Item data1, Item data2);
int ITEMgreater(Item data1, Item data2);

int KEYcompare(Key k1, Key k2);
Key KEYget(Item data);

// getter / setter
char *getName(Item i);
int getPoints(Item i);
void setPoints(Item i, int points);

#endif

