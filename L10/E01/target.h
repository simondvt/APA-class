#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED


typedef enum {ALLEATO, NEMICO} targetT;

typedef struct tar
{
    int r, c;
    int v;
    targetT tipo_target;    // etichetta che mi dice se il target è alleato o nemico
    struct tar *next;
} *Target;

// legge un target da file e lo inserisce in lista
// nel formato r c v TIPO, con TIPO 'a' o 'n'
Target insTarget(Target head, FILE *fp);

#endif // TARGET_H_INCLUDED
