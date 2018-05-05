#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Item.h"

struct item { char *name; int value; };

Item ITEMscan(FILE *fp) {
  char name[MAXC];

  if (fscanf(fp, "%s", name) == EOF)
    return NULL;

  Item tmp = (Item) malloc(sizeof(struct item));

  // controllo NULL nel main
  //if (tmp == NULL)
    //return ITEMsetvoid();

 // else {
    tmp->name = strdup(name);
    tmp->value = 10;
  //}
  return tmp;
}

void ITEMshow(Item data, FILE *fp) {
  fprintf(fp, "Nome = %s\nPunti = %d\n", data->name, data->value);
}

Item ITEMsetvoid() {
  char name[MAXC]="";
  Item tmp = (Item) malloc(sizeof(struct item));
  if (tmp != NULL) {
    tmp->name = strdup(name);
    tmp->value = -1;
  }
  return tmp;
}

int ITEMless (Item data1, Item data2) {
  Key k1 = KEYget(data1), k2 = KEYget(data2);
  if (KEYcompare(k1, k2) == -1)
    return 1;
  else
    return 0;
}

int ITEMgreater(Item data1, Item data2) {
  Key k1 = KEYget(data1), k2 = KEYget(data2);
  if (KEYcompare(k1, k2) == 1)
    return 1;
  else
    return 0;
}

int  KEYcompare(Key k1, Key k2) {
  if (k1 < k2)
    return -1;
  else if ( k1 == k2)
    return 0;
  else
    return 1;
}

Key KEYget(Item data) {
  return data->value;
}

char *getName(Item i)
{
    return i->name;
}

int getPoints(Item i)
{
    return i->value;
}

void setPoints(Item i, int points)
{
    i->value = points;
}
