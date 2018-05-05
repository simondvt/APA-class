#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_FILE "prodotti.txt"
#define MAX_STR 25

typedef struct
{
    char *nome;
    float prezzo;
} Prodotto;

// legge dal file i prodotti e li salva nel vettore di
// struct "prodotto" allocato dinamicamente
int leggi(Prodotto **prodotto);

// trova tutti gli insiemi che rispettano i vincoli di budget
int comb_sempl(int pos, Prodotto *val, char **sol, int n, int k, int start, int count, float budget, float currentBudget);

int main(void)
{
    int i, n, k, count;
    float budget;    // budget
    char **sol = NULL; // array di stringhe soluzione
    Prodotto *prodotto;

	n = leggi(&prodotto);

	printf("K: ");
	scanf("%d", &k);
	printf("B: ");
	scanf("%f", &budget);

	// alloco lo spazio per n insiemi, tanto come ultimo caso sol sarà lungo n
    sol = malloc(k * sizeof(*sol));
    if (sol == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

	// ciclo da k in su, rispettato il vincolo di cardinalità
	while (k < n)
    {
        count = comb_sempl(0, prodotto, sol, n, k, 0, 0, budget, 0);
        printf("Trovati %d insiemi per k = %d\n\n", count, k);
        k++;
    }


    // libero la memoria
    for (i = 0; i < n; i++)
        free(prodotto[i].nome);
    free(prodotto);
    free(sol);

    return 0;
}


int leggi(Prodotto **prodotto)
{
    FILE *fp;
    int i, n;
    float prezzo;
    char buf[MAX_STR + 1];

    if ((fp = fopen(SRC_FILE, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", SRC_FILE);
        exit(1);
    }

    fscanf(fp, "%d\n", &n);

    if ((*prodotto = malloc(n * sizeof(**prodotto))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%s %f", buf, &prezzo);

        (*prodotto)[i].prezzo = prezzo;
        (*prodotto)[i].nome = malloc((strlen(buf) + 1) * sizeof(*((*prodotto)[i].nome)));

        if ((*prodotto)[i].nome == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(1);
        }

        strcpy((*prodotto)[i].nome, buf);
    }

    return n;
}

int comb_sempl(int pos, Prodotto *val, char **sol, int n, int k, int start, int count, float budget, float currentBudget)
{
	int i;

    // sol contiene una soluzione
	if (pos >= k)
	{
        printf("{");
        for (i = 0; i < k; i++)
            printf(" %s", sol[i]);

        printf(" }");
        printf("\n");

        return count + 1;
	}

	for (i = start; i < n; i++)
	{
	    sol[pos] = val[i].nome;

		if (currentBudget + val[i].prezzo <= budget)    // pruning
            count = comb_sempl(pos + 1, val, sol, n, k, i + 1, count, budget, currentBudget + val[i].prezzo);
	}

	return count;
}
