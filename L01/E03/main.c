#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LEN (200 + 1)       // al massimo una parola occuperà tutta una riga
#define SOSTITUZIONI 30

#define SRC "src.txt"
#define SOST "sost.txt"
#define DEST "dest.txt"

FILE *src, *sost, *dest;
int s;

// struct per memorizzare il contenuto del file SOST
struct
{
    int n;
    char s[LEN];
} sostituzioni[SOSTITUZIONI];


// funzione che ricevuta una stringa, cerca se è disponibile una sostituzione,
// in caso positivo, stampa la sostituzione, altrimenti stampa la parola originale.
void codifica(char *stringa);

int main(void)
{
    int i;
    int count = 0;  // contatore della parola in esame
    char parola[LEN];
    char ch;

    if ((src = fopen(SRC, "r")) == NULL)
    {
        printf("Impossibile aprire %s!\n", SRC);
        return 1;
    }
    if ((sost = fopen(SOST, "r")) == NULL)
    {
        printf("Impossibile aprire %s!\n", SOST);
        return 1;
    }
    if ((dest = fopen(DEST, "w")) == NULL)
    {
        printf("Impossibile scrivere su %s!\n", DEST);
        return 1;
    }

    printf("Codifico %s usando %s salvando su %s.\n", SRC, SOST, DEST);

    fscanf(sost, "%d\n", &s);

    for (i = 0; i < s; i++)
        fscanf(sost, "%d %s\n", &sostituzioni[i].n, sostituzioni[i].s);

    fclose(sost);


    // leggo carattere per carattere costruendomi la stringa,
    // in questo modo posso ristampare le parole mantenendo gli stessi
    // segni di interpunzione e di formattazione (\t \n) del file originale.
    while ((ch = fgetc(src)) != EOF)
    {
        if (isalpha(ch))
        {
            parola[count++] = ch;
        }
        else
        {
            parola[count] = '\0';
            count = 0;
            codifica(parola);
            fprintf(dest, "%c", ch);    // stampa del carattere letto dopo la fine della parola
        }
    }

    fclose(src); fclose(dest);

    printf("Codifica effettuata con successo.\n");

    return 0;
}

void codifica(char *stringa)
{
    int i;

    // se la lunghezza della parola è uguale all n di uno di una delle sostituzioni,
    // stampo la sostituzione corrispondente
    for (i = 0; i < s; i++)
    {
        if (strlen(stringa) == sostituzioni[i].n)
        {
            fprintf(dest, "%s", sostituzioni[i].s);
            return ;    // evito di controllare altre sostituzioni
        }
    }

    fprintf(dest, "%s", stringa);
}
