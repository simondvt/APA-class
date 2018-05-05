#include <stdio.h>
#include <stdlib.h>
#include "target.h"

static Target newTarget(int r, int c, int v, targetT tipo)
{
    Target t = malloc(sizeof(*t));
    t->r = r;
    t->c = c;
    t->v = v;
    t->tipo_target = tipo;
    t->next = NULL;

    return t;
}

Target insTarget(Target head, FILE *fp)
{
    int v, r, c;
    char tipoC;
    targetT tipo;
    Target new;

    if (fscanf(fp, "%d %d %d %c", &r, &c, &v, &tipoC) == EOF)
        return NULL;

    tipo = tipoC == 'a' ? ALLEATO : NEMICO;
    new = newTarget(r, c, v, tipo);
    new->next = head;
    return new;
}