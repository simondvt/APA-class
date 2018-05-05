#include <stdio.h>
#include <stdlib.h>


int H(int n, int **memo, int *size);

int main(void)
{
    int x, i;
    int *memo = NULL;
    int size = 10;      // 10 valore di partenza arbitrario

    // inizializzo il vettore per la memoization
    // devo azzerare perché il test è fatto su un elemento nullo
    if ((memo = calloc(size, sizeof(*memo))) == NULL)
    {
        printf("Memoria insufficiente!\n");
        return 1;
    }

    printf("Inserisci un intero positivo: ");
    scanf("%d", &x);
    printf("I primi %d termini della sequenza di Hofstadter sono:", x);

    for (i = 0; i < x; i++)
    {
        // solo per scopi di formattazione
        if (i % 20 == 0) printf("\n");

        printf("%d, ", H(i, &memo, &size));
    }

    printf("\n");
    free(memo);
    return 0;
}


int H(int n, int **memo, int *size)
{
    // variabile ausiliara, se la realloc fallice ho ancora
    // un aggancio a memo, su cui posso richiamare la free
    int *memoSafe;
    int i;

    if (*size <= n)
    {
        // il fattore moltiplicativo è arbitrario
        *size *= 2;

        if ((memoSafe = realloc(*memo, *size * sizeof(*memoSafe))) == NULL)
        {
            printf("Memoria insufficiente!\n");
            free(*memo);
            exit(1);
        }

        *memo = memoSafe;

        // devo azzerare perché il test è fatto su un elemento nullo
        for (i = n; i < *size; i++)
            (*memo)[i] = 0;
    }


    // caso di terminazione
    if (n == 0)
        return (*memo)[0];

    // se ho già calcolato l' n-esimo numero, lo prendo dall'array
    // altrimenti lo calcolo e lo salvo nell array
    else
    {
        if ((*memo)[n] != 0)
            return (*memo)[n];
        else
        {
            (*memo)[n] = n - H(H(H(n - 1, memo, size), memo, size), memo, size);
            return (*memo)[n];
        }
    }
}

/*
* H senza memoization
*   int H(int n)
*   {
*       if (n == 0) return 0;
*
*       return n - H(H(H(n - 1)));
*   }
*/
