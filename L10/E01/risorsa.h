#ifndef RISORSA_H_INCLUDED
#define RISORSA_H_INCLUDED


typedef struct
{
    int r, a, d, c;
} *Risorsa;

// leggi da file nel formato r a d c
Risorsa leggiRisorsa(FILE *fp);
// costruttore
Risorsa newRisorsa(int r, int a, int d, int c);

#endif // RISORSA_H_INCLUDED
