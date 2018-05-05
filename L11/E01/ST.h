#ifndef ST_H_INCLUDED
#define ST_H_INCLUDED

typedef struct symboltable *ST;

ST STinit(int max);
int STinsert(ST st, char *str);
int STsearch(ST st, char *k);
void STdisplay(ST st);
int STcount(ST st);
char *STkey(ST st, int i);
void deleteST(ST st);

#endif // ST_H_INCLUDED
