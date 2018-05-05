/*
*   Lab00, E02
*   Manipolazione di stringhe
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int consonanti(char *s)
{
    int count = 0, i;

    for (i = 0; s[i] != '\0'; i++)
    {
        switch (toupper(s[i]))
        {
        case 'A': case 'E': case 'I': case  'O': case 'U':
            break; // non fare nulla
        default:
            count++;
        }
    }

    return count;
}

int main(void)
{
    int N, i;
    char P[20], parola[20];
    FILE *originale, *uno, *due;
    char originaleF[10], unoF[10], dueF[10];

    printf("Inserire il nome dei tre file: ");
    scanf("%s %s %s", originaleF, unoF, dueF);

    if ((originale = fopen(originaleF, "r")) == NULL)
    {
        printf("Errore in apertura di %s.\n", originaleF);
        return 0;
    }

    if ((uno = fopen(unoF, "w")) == NULL)
    {
        printf("Errore in apertura di %s.\n", unoF);
        return 0;
    }

    if ((due = fopen(dueF, "w")) == NULL)
    {
        printf("Errore in apertura di %s.\n", dueF);
        return 0;
    }

    fscanf(originale, "%d %s\n", &N, P);

    for (i = 0; i < N; i++)
    {
        fscanf(originale, "%s\n", parola);

        // file uno
        if (strcmp(P, parola) < 0)
            fprintf(uno, "%s\n", parola);

        // file due
        if (consonanti(parola) > consonanti(P))
            fprintf(due, "%s\n", parola);
    }

    fclose(originale); fclose(uno); fclose(due);

    return 0;
}
