#ifndef _DATO_INCLUDED
#define _DATO_INCLUDED

#define MAXC 35

#include <stdio.h>

typedef struct item* Item;
typedef char *Key;

Item newITEM(Key k, int n);
Item ITEMscan();
void ITEMshow(Item data, FILE *fp);
int ITEMcheckvoid(Item data);
Item ITEMsetvoid();
int ITEMless(Item data1, Item data2);

Key KEYscan(FILE *fp);
int KEYcompare(Key k1, Key k2);
Key KEYget(Item data);
void ITEMfree(Item i);
#endif

