#include <stdio.h>
#include <stdlib.h>
#include "risorsa.h"

Risorsa newRisorsa(int r, int a, int d, int c)
{
    Risorsa ris = malloc(sizeof(*ris));
    ris->r = r;
    ris->a = a;
    ris->d = d;
    ris->c = c;

    return ris;
}

Risorsa leggiRisorsa(FILE *fp)
{
    int r, a, d, c;

    fscanf(fp, "%d %d %d %d", &r, &a, &d, &c);

    return newRisorsa(r, a, d, c);
}