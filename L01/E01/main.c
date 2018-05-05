#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> // per la funzione exit()

#define N 20
#define N2 (2 * N)  // sovralloco, al massimo l'unione sarà di 2n caselle

int V1[N], V2[N];   // vettori di input
int V[N2];          // vettore risultante
int n1, n2;         // dimensioni effettive di V1 e V2

void menu(void);        // mostra un menu basato su singolo carattere
int cerca(int *v, int n, int chiave);  // controlla se un elemento è presente nel vettore, ritorna bool
void stampa(int *v, int n);     // stampa il vettore

void unione(void);
void differenza(void);
void intersezione(void);

int main(void)
{
    n1 = 0, n2 = 0;

    // input non molto elegante, ma semplifica la vita all'utente
    printf("Inserire elementi per primo vettore (lettera per terminare): ");
    while (n1 < 20 && scanf("%d", &V1[n1++]) == 1);
    fflush(stdin);
    n1--;

    printf("Inserire elementi per secondo vettore (lettera per terminare): ");
    while (n2 < 20 && scanf("%d", &V2[n2++]) == 1);
    fflush(stdin);
    n2--;

    menu();

    // per sopprimere un warning, l'uscita viene effettuata mediante menu
    return 0;
}

void menu(void)
{
    char scelta;

    printf("Scelte disponibili: \n");
    printf("U - Unione insiemistica\nD - Differenza insiemistica\nI - Intersezione insiemistica\nE - Esci\n");
    scanf(" %c", &scelta);

    switch (toupper(scelta))
    {
        case 'U': unione();         break;
        case 'D': differenza();     break;
        case 'I': intersezione();   break;
        case 'E': exit(0);  // la exit è necessaria perché NON sono dentro al main
        default :
            printf("Scelta non valida!\n");
            menu();
    }
}

int cerca(int *v, int n, int chiave)
{
    int i;

    for (i = 0; i < n; i++)
        if (v[i] == chiave)
            return 1;

    return 0;
}

void stampa(int *v, int n)
{
    int i;

    for (i = 0; i < n; i++)
        printf(" %d", v[i]);

    printf("\n");
}

void intersezione(void)
{
    int i, j;

    // numero elementi vettore V
    // dopo la funzione V avrà indici che vanno da 0 a c-1
    int c = 0;

    // se ogni elemento di V1 è contenuto anche in V2 ma non è già
    // stato inserito in V, allora lo inserisco
    for (i = 0; i < n1; i++)
        for (j = 0; j < n2; j++)
            if (cerca(V2, n2, V1[i]) && !cerca(V, c, V1[i]))
                V[c++] = V1[i];


    printf("L'intersezione e':");
    stampa(V, c);
    menu();
}

void differenza(void)
{
    int i;

    // numero elementi vettore V
    // dopo la funzione V avrà indici che vanno da 0 a c-1
    int c = 0;


    // se ogni elemento di V1 NON è contenuto in V2 e
    // non è già stato inserito in V, allora lo inserisco
    for (i = 0; i < n1; i++)
        if (!cerca(V, c, V1[i]) && !cerca(V2, n2, V1[i]))
            V[c++] = V1[i];


    // se ogni elemento di V2 NON è contenuto in V2 e
    // non è già stato inserito in V, allora lo inserisco
    for (i = 0; i < n2; i++)
        if (!cerca(V, c, V2[i]) && !cerca(V1, n1, V2[i]))
            V[c++] = V2[i];

    printf("La differenza e':");
    stampa(V, c);
    menu();
}

void unione(void)
{
    int i;

    // numero elementi vettore V
    // dopo la funzione V avrà indici che vanno da 0 a c-1
    int c = 0;


    // se ogni elemento di V1 NON è già stato inserito
    // in V, allora lo inserisco
    for (i = 0; i < n1; i++)
        if (!cerca(V, c, V1[i]))
            V[c++] = V1[i];


    // se ogni elemento di V2 NON è già stato inserito
    // in V, allora lo inserisco
    for (i = 0; i < n2; i++)
        if (!cerca(V, c, V2[i]))
            V[c++] = V2[i];


    printf("L'unione e':");
    stampa(V, c);
    menu();
}
