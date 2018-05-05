#include <stdio.h>

#define SIZE 30

int mediaMobile(int V[], int v, int k, float M[]);

int main(void)
{
    int V[SIZE];
    float M[SIZE] = {0};
    int k;
    int medie;      // numero di medie calcolate
    int size = 0;   // dimensione effettiva del vettore V
    int i;

    printf("Inserire elementi del vettore (lettera per terminare): ");
    while (size < SIZE && scanf("%d", &V[size++]) == 1);
    fflush(stdin);
    size--;

    printf("Inserire k: ");
    scanf("%d", &k);

    medie = mediaMobile(V, size, k, M);
    printf("Sono state calcolate %d medie e il vettore M si presenta come:\n", medie);
    for (i = 0; i < medie; i++)
        printf("%.3f ", M[i]);

    return 0;
}

int mediaMobile(int V[], int v, int k, float M[])
{
    int medie, i;

    // per ogni casella, esamino le k caselle successive (quando possibile),
    // calcolando la loro media
    for (medie = 0; medie + k <= v; medie++)
    {
        for (i = medie; i < medie + k; i++)
            M[medie] += V[i];

        M[medie] /= k;
    }

    return medie;
}
