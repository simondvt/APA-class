#include <stdio.h>

int somma(int n);

int main(void)
{
    int x;

    printf("Inserisci un intero non negativo: ");
    scanf("%d", &x);

    printf("somma(%d) = %d\n", x, somma(x));
    return 0;
}

int somma(int n)
{
    // se n è composto da una singola cifra
    if (n / 10 == 0) return n;

    // n % 10 estrae l'ultima cifra
    return n % 10 + somma(n / 10);
}
