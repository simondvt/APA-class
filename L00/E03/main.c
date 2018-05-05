/*
*   Lab00, E03
*   Integrazione numerica mediante metodo dei rettangoli
*/

#include <stdio.h>
#include <math.h>

float valutaHorner(float P[11], float x)
{
    int i;
    float result = 0;

    for (i = 11; i > 0; i--)
    {
        result = result * x + P[i - 1];
    }


    return result;
}

int main(void)
{
    float P[11] = {46, 12, -9, 1, 0};
    printf("%.2f\n", valutaHorner(P, 50));

    int i, n;
    int a, b, M;
//    float P[11] = {0};

//    printf("Inserisci il grado del polinomio (n <= 10): ");
//    scanf("%d", &n);
//
//    for (i = 0; i < n + 1; i++)
//    {
//        printf("Inserisci il coefficiente %d: ", i + 1);
//        scanf("%f", &P[i]);
//    }

    printf("Inserire a e b: ");
    scanf("%d %d", &a, &b);

    printf("Inserire M: ");
    scanf("%d", &M);

    float h = (float)(b - a) / M;
    int k;
    float sum = 0;
    float x = a;
    float y;

    for (k = 0; k < M; k++)
    {
        y = x + h;
        sum += valutaHorner(P, (y + x) / 2);
        x=y;
    }

    sum *= h;

    printf("\nL'integrale vale %.2f\n", sum);
    return 0;
}
