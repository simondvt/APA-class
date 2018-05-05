#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FILE_IN "seq.txt"

#define DX      0
#define SX      1

// powerset comincia ad elencare da insiemi di cardinalità minore
// quindi posso controllare solo se la sequenza è equilibrata
// perché so che alla prima occorrenza di una soluzione valida
// essa sarà quella a cardinalità minima
int powerset_r(int *val, int k, int *sol, int n, int pos, int start, int *v);
int powerset(int *val, int k, int *sol, int *v);
// ritorna 1 se la sequenza è equilibrata, 0 altrimenti
int equilibrato(int *v, int n);
// stampa la sequenza
void printVal(int *v, int n);
// gira le frecce
void gira(int *v, int *sol, int n);

// soluzioni DP
// DP bottom up, trova il valore e la soluzione
void DP_bottomUp(int *v, int n);
int equilibra(int *v, int l, int r);
void equilibra_e_cambia(int *v, int l, int r, int *best);
// DP basata su memoization, TROVA SOLO IL VALORE
int DPR(int *v, int n);
int DPR_wrap(int *v, int l, int r, int *memo);


int main(void)
{
    int i;
    int *v, n;
    FILE *fp = fopen(FILE_IN, "r");
    int *val, *sol;

    if (fp == NULL)
    {
        printf("Errore in lettura di %s\n", FILE_IN);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d\n", &n);
    v = malloc(n * sizeof(*v));
    sol = malloc(n * sizeof(*sol));
    val = malloc(n * sizeof(*val));

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d", &v[i]);
        val[i] = i;
    }

    printf("Letta sequenza:\n");
    printVal(v, n);

    printf("\nBottom Up:\n");
    DP_bottomUp(v, n);

    printf("\nMemoization:\nMin Costo: %d\n", DPR(v, n));

    printf("\nCalcolo Combinatorio:\n");
    if (!powerset(val, n, sol, v))
        printf("Soluzione NON trovata.\n");


    free(v);
    free(sol);
    free(val);
    fclose(fp);
    return 0;
}

int equilibrato(int *v, int n)
{
    // l'algoritmo è O(n)

    int i, j, k;
    int count;

    i = 0;
    while (i < n)
    {
        // la prima freccia di una sottosequenza deve essere per forza DX
        if (v[i] == SX)
            return 0;

        // determino quante frecce DX ci sono
        count = 1;
        for (j = i + 1; j < n && v[j] == DX; j++)
        {
            if (v[j] == DX)
                count++;
            else
                break;
        }

        // per tante volte mi assicuro che ce ne siamo altrettante SX
        for (k = 0; j < n && k < count; k++, j++)
        {
            if (v[j] == DX)
                return 0;
        }

        if (k < count)
            return 0;

        i = j;
    }

    return 1;
}

int powerset_r(int *val, int k, int *sol, int n, int pos, int start, int *v)
{
	int i;

	if (pos >= n)
	{
		gira(v, sol, n);

        if (equilibrato(v, k))
        {
            printf("Soluzione trovata scambiando %d frecce.\n", n);
            printVal(v, k);

            return 1;
        }
        else
        {
            // backtrack
            gira(v, sol, n);

            return 0;
        }
	}

	for (i = start; i < k; i++)
	{
		sol[pos] = val[i];
		if (powerset_r(val, k, sol, n, pos + 1, i + 1, v))
            return 1;
	}

	return 0;
}

int powerset(int *val, int k, int *sol, int *v)
{
	int n;

	for (n = 0; n <= k; n++)
		if (powerset_r(val, k, sol, n, 0, 0, v))
            return 1;

	return 0;
}

void printVal(int *v, int n)
{
    int i;

    printf("Sequenza: ");
    for (i = 0; i < n; i++)
        //printf("%s\t", v[i] == DX ? "->" : "<-");
        //printf(" %d", v[i] == DX ? DX : SX);
        printf(" %d", v[i]);
    printf("\n");
}

void gira(int *v, int *sol, int n)
{
    int i;

    for (i = 0; i < n; i++)
        v[sol[i]] = 1 - v[sol[i]];  // swap tra 0 e 1 (DX e SX)
}

void DP_bottomUp(int *v, int n)
{
    /*
    *   n / 2 sotto-problemi, per ognuno (n - i) / 2 scelte
    *   ==> algoritmo O(n^2)    // solo valore
    *   ==> algoritmo O(n^3)    // valore e soluzione
    */

    int i, j;
    int q;  // costo attulae
    int *best;
    int *sol;

    // NON tutte le caselle di best sono utilizzate
    best = malloc((n + 1) * sizeof(int));
    sol = malloc(n * sizeof(int));

    for (i = 0; i < n + 2; i++)
        best[i] = 0;

    for (i = 0; i < n; i++)
        sol[i] = v[i];

    for (i = n - 2; i >= 0; i -= 2)
    {
        best[i] = INT_MAX;
        for (j = i + 1; j < n; j += 2)
        {
            q = equilibra(v, i, j) + best[j + 1];
            if (q < best[i])
            {
                best[i] = q;
                equilibra_e_cambia(v, i, j, sol);
            }
        }
    }

    printf("Min Costo = %d\n", best[0]);

    printVal(sol, n);

    free(best);
    free(sol);
}

int DPR(int *v, int n)
{
	int *memo;
	int esito, i;

	memo = malloc(n * sizeof(int));

	for (i = 0; i < n; i++)
    {
        memo[i] = -1;
    }

	esito = DPR_wrap(v, 0, n - 1, memo);

	free(memo);
	return esito;
}

int DPR_wrap(int *v, int l, int r, int *memo)
{
    int i;
    int costo = 0;
	int minCosto = INT_MAX;

	if (l > r) // caso terminale
    {
        return 0;
    }

    for (i = l + 1; i <= r; i += 2)
    {
        costo = equilibra(v, l, i);

        if (memo[i] == -1)
        {
            memo[i] = DPR_wrap(v, i + 1, r, memo);
        }

        costo += memo[i];

        if (costo < minCosto)
		{
            minCosto = costo;
		}
    }

	return minCosto;
}

int equilibra(int *v, int l, int r)
{
	// algoritmo O(n)

    int i, girate = 0;


    for (i = l; i <= (l + r) / 2; i++)
    {
        if (v[i] == SX)
        {
            girate++;
        }
    }

    for (i = (l + r) / 2 + 1; i <= r; i++)
    {
        if (v[i] == DX)
        {
            girate++;
        }
    }

    return girate;
}

void equilibra_e_cambia(int *v, int l, int r, int *best)
{
    int i;

    for (i = l; i <= (l + r) / 2; i++)
    {
        best[i] = DX;
    }

    for (i = (l + r) / 2 + 1; i <= r; i++)
    {
        best[i] = SX;
    }
}
