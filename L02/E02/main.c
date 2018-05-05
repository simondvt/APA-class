#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define SEQUENZE "sequenze.txt"
#define TESTO    "testo.txt"

int sequenzeNum;

// aggiorna la struct sequenze[]
void processaParola(char *s, int n);

/* la struct contiene:
*
* -parola: stringa che contiene la sequenza dal file sequenze.txt
* -occorrenze: array di stringhe, contiene tutte le parole del file che cominciano
*              con la sequenza + il numero di parola (uso di sprintf)
* -occorrenzeNum: contatore per ricordarmi quante occorrenze ho trovato
*/
struct
{
    char parola[5 + 1];

    // potenzialmente tutte le parole del file possono essere
    // un'occorrenza. Sovralloco; altrimenti potrei leggere una
    // prima volta il file, contare le parole e poi
    // allocare dinamicamente.
    char occorrenze[25 + 1 + 5][1000]; // 5 caratteri in più per il numero di parola
    int occorrenzeNum;
} sequenze[20];

int main(void)
{
    char parola[25 + 1] = {0};
    FILE *sequenzeF, *testoF;
    int parolaCount = 0;   // numero di parola
    char c;
    int i, j;

    if ((sequenzeF = fopen(SEQUENZE, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", SEQUENZE);
        return 1;
    }
    if ((testoF = fopen(TESTO, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", TESTO);
        return 1;
    }

    // leggo il file sequenze
    fscanf(sequenzeF, "%d", &sequenzeNum);
    for (i = 0; fscanf(sequenzeF, "%s", sequenze[i].parola) != EOF; i++)
        sequenze[i].occorrenzeNum = 0; // inizializzazione

    fclose(sequenzeF);

    // leggo una parola, e appena letta la
    // passo alla funzione processaParola
    i = 0;
    while (fscanf(testoF, "%c", &c) != EOF)
    {
        if (isalnum(c))
        {
            parola[i++] = c;
        }
        else
        {
            parola[i] = '\0';
            i = 0;

            // controllo necessario, altrimenti conta gli spazi e i segni
            // d'interpunzione come parole, sfalsando parolaCount
            if (strlen(parola) != 0)
            {
                parolaCount++;
                processaParola(parola, parolaCount);
            }
        }
    }

    fclose(testoF);


    // stampo risultati
    for (i = 0; i < sequenzeNum; i++)
    {
        // istruzione aggiuntiva, utile solo per un output
        // più pulito, se non ho occorrenze non stampo
        // nulla di quella sequenza
        if (sequenze[i].occorrenzeNum == 0) continue;


        printf("%s\n", sequenze[i].parola);

        for (j = 0; j < sequenze[i].occorrenzeNum; j++)
            printf("%s\n", sequenze[i].occorrenze[j]);

        printf("\n");
    }

    return 0;
}


void processaParola(char *s, int n)
{
    int i;
    int count;      // variabile ausiliara per non scrivere un nome molto lungo

    for (i = 0; i < sequenzeNum; i++)
    {
        if (strnicmp(s, sequenze[i].parola, strlen(sequenze[i].parola)) == 0)
        {
            count = sequenze[i].occorrenzeNum++;
            // faccio attenzione ad includere '\0' perché sto scrivendo su una stringa
            sprintf(sequenze[i].occorrenze[count], "%s %d%c", s, n, '\0');
            break;
        }
    }
}
