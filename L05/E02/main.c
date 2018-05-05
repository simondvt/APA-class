#include <stdio.h>

#define N 2
#define MAXS 20
#define SRCFILE "src.txt"

typedef struct item_s
{
    int a, b;
    char c;
    float d;
    char s[MAXS];
} Item;

int main(void)
{
    Item M[N][N];
    FILE *fp;

    int count = 0;  // contatore di quanti Item ho letto
    int i, j;       // indici matrice

    if ((fp = fopen(SRCFILE, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", SRCFILE);
        return 1;
    }

    while (count < N * N)
    {
        i = count / N;
        j = count % N;

        fscanf(fp, "%d%d %c%f%s",
               &M[i][j].a, &M[i][j].b, &M[i][j].c, &M[i][j].d, M[i][j].s);

        count++;
    }

    printf("Inserire riga e colonna (indici partono da 0): ");
    scanf("%d%d", &i, &j);

    if (i < 0 || i >= N || j < 0 || j >= N)
    {
        printf("Indici non validi!\n");
        return 1;
    }

    printf("M[%d][%d]:\n", i, j);

    printf("a: %d\t\tat %p\n", M[i][j].a, &M[i][j].a);
    printf("b: %d\t\tat %p\n", M[i][j].b, &M[i][j].b);
    printf("c: %c\t\tat %p\n", M[i][j].c, &M[i][j].c);
    printf("d: %.2f\t\tat %p\n", M[i][j].d, &M[i][j].d);
    printf("s: %s\t\tat %p\n", M[i][j].s, &M[i][j].s);


    return 0;
}
