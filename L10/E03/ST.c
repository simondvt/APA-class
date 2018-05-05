#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ST.h"

struct symboltable
{
    char **str;
    int N, M;
};

ST STinit(int max)
{
    int i;
    ST st = malloc(sizeof(*st));

    st->N = 0;
    st->M = max;
    st->str = malloc(max * sizeof(*st->str));

    for (i = 0; i < st->M; i++)
        st->str[i] = NULL;

    return st;
}

static int isFull(ST st, int i)
{
    return (st->str[i] != NULL);
}

static int hash(char *k, int M)
{
    int h = 0, base = 127;

    for ( ; *k != '\0'; k++)
        h = (base * h + *k -64) % M;

    return h;
}

int STinsert(ST st, char *str)
{
    int i, startI;

    startI = i = hash(str, st->M);
    while(isFull(st, i))
    {
        i = (i + 1) % st->M;
        if (i == startI)
        {
            printf("[ERR] spazio esaurito nella tabella di hash.\n");
            exit(EXIT_FAILURE);
        }
    }

    st->str[i] = str;
    st->N++;

    return i;
}

int STsearch(ST st, char *k)
{
    int i;

    i = hash(k, st->M);

    while (isFull(st, i))
    {
        if (strcmp(k, st->str[i]) == 0)
            return i;
        else
            i = (i + 1) % st->M;
    }

    return -1;
}

void STdisplay(ST st)
{
    int i;

    for (i = 0; i < st->M; i++)
        printf("%s\n", st->str[i]);
}

int STcount(ST st)
{
    return st->N;
}

char *STkey(ST st, int i)
{
    return st->str[i];
}