/*
*   Lab00, E01
*   Medie per riga e per colonna
*/

#include <stdio.h>

int main(void)
{
    FILE *fp;
    char file[10];
    int n = 0, i;
    int col[5] = {0}, dato[5];

    printf("Inserire nome file: ");
    gets(file);

    if ((fp = fopen(file, "r")) == NULL)
    {
        printf("ERRORE\n");
        return 0;
    }

    while (fscanf(fp, "%d %d %d %d %d\n", &dato[0], &dato[1], &dato[2], &dato[3], &dato[4]) != EOF)
    {
        for(i = 0; i < 5; i++)
            col[i] += dato[i];

        n++;

        printf("Media riga %d: %f\n", n, (float)(dato[0] + dato[1] + dato[2] + dato[3] + dato[4]) / 5);
    }
    fclose(fp);

    for (i = 0; i < 5; i++)
        printf("Media colonna %d: %f\n", i + 1, (float)(col[i]) / n);

    return 0;
}
