#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_IN "elementi.txt"
#define N_ELEMENTI 8
#define BONUS 2.5

typedef enum
{
    NA, AA, AI
} Tipo;

typedef struct
{
    char *nome;
    int grado;
    float punti;
    Tipo tipo;
} *elemento;

typedef struct
{
    elemento *v;
    float punteggio;
    int sum;
    elemento flag2cat, flaglastcat;
} soluzione;

typedef struct
{
    int na, aa, ai;
    elemento *elementi;
} *tab_elementi;

typedef struct
{
    int maxNA, maxAA, maxAI, maxSUM;
} atleta;

// legge e ritorna gli elementi letti da file
tab_elementi leggiElementi(FILE *fp);
// mostra la soluzione
void displaySol(soluzione *best);
// controllo elemento ripetuto
int ripetuto(soluzione *sol, int pos, elemento el);

// algoritmo completo
void complete(tab_elementi elementi, atleta a);
int comb_rip(int pos, int start, soluzione *sol, int n, int k, int count, tab_elementi te, atleta a, soluzione *best);
// controllo vincoli difficoltà
int verifica(soluzione *sol, atleta a, elemento el);
// controllo trovata soluzione migliore
void check_best(soluzione *sol, soluzione *best);
// controllo elementi di almeno due categorie diverse
int cat2(soluzione *sol, int pos, elemento el);

// algoritmo greedy
void greedy(tab_elementi elementi, atleta a);
// confronta gli elementi in base al rapporto punti/grado (ordine decrescente) (per qsort)
int compare(const void *e1, const void *e2);
// se greedy non ce la fa
void greedy_aux(tab_elementi te, atleta a);


int main(void)
{
    tab_elementi te;
    FILE *fp;

    atleta a;

    if ((fp = fopen(FILE_IN, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", FILE_IN);
        exit(EXIT_FAILURE);
    }

    te = leggiElementi(fp);
    fclose(fp);

    printf("Inserire capacita' atleta: ");
    scanf("%d %d %d %d", &a.maxNA, &a.maxAA, &a.maxAI, &a.maxSUM);

    printf("\nALGORITMO COMPLETO\n");
    complete(te, a);

    printf("\nALGORITMO GREEDY\n");
    greedy(te, a);

    return 0;
}

tab_elementi leggiElementi(FILE *fp)
{
    int i;
    int na, aa, ai;
    elemento *elementi;
    elemento curElemento;
    tab_elementi te = malloc(sizeof(*te));

    char buf[100];
    int grado;
    float punti;

    fscanf(fp, "%d %d %d", &na, &aa, &ai);
    if ((elementi = malloc((na + aa + ai) * sizeof(*elementi))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < na; i++)
    {
        fscanf(fp, "%s %d %f", buf, &grado, &punti);

        if ((curElemento = malloc(sizeof(*curElemento))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }

        curElemento->nome = strdup(buf);
        curElemento->grado = grado;
        curElemento->punti = punti;
        curElemento->tipo = NA;

        elementi[i] = curElemento;
    }

    for (i = na; i < na + aa; i++)
    {
        fscanf(fp, "%s %d %f", buf, &grado, &punti);

        if ((curElemento = malloc(sizeof(*curElemento))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }

        curElemento->nome = strdup(buf);
        curElemento->grado = grado;
        curElemento->punti = punti;
        curElemento->tipo = AA;

        elementi[i] = curElemento;
    }

    for (i = na + aa; i < na + aa + ai; i++)
    {
        fscanf(fp, "%s %d %f", buf, &grado, &punti);

        if ((curElemento = malloc(sizeof(*curElemento))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }

        curElemento->nome = strdup(buf);
        curElemento->grado = grado;
        curElemento->punti = punti;
        curElemento->tipo = AI;

        elementi[i] = curElemento;
    }

    te->aa = aa;
    te->ai = ai;
    te->na = na;
    te->elementi = elementi;

    return te;
}

void displaySol(soluzione *best)
{
    int i;
    int found2cat = 0;

    printf("Migliore punteggio: %.2f\n\n", best->punteggio);

    for (i = 0; i < N_ELEMENTI; i++)
    {
        if (!ripetuto(best, i, best->v[i]))
            printf("%s: %.2f\n", best->v[i]->nome, best->v[i]->punti);
        else
            printf("%s: %.2f\n", best->v[i]->nome, 0.0f);

        if (!found2cat && best->flag2cat == best->v[i])
        {
            printf("[BONUS]Esigenza composizione: %.2f\n", BONUS);
            found2cat = 1;
        }
    }

    if (best->flaglastcat != NULL)
        printf("[BONUS]Esigenza uscita: %.2f\n", BONUS);
}

/* COMPLETO */

void complete(tab_elementi te, atleta a)
{
    int n;
    soluzione *sol, *best;

    n = te->aa + te->ai + te->na;
    sol = malloc(sizeof(*sol));
    best = malloc(sizeof(*best));
    sol->v = malloc(N_ELEMENTI * sizeof(*(sol)->v));
    best->v = malloc(N_ELEMENTI * sizeof(*(best)->v));

    best->punteggio = 0;
    sol->punteggio = 0;
    sol->flag2cat = NULL;
    sol->flaglastcat = NULL;


    comb_rip(0, 0, sol, n, N_ELEMENTI, 0, te, a, best);

    displaySol(best);

    free(sol);
    free(best);
}

int verifica(soluzione *sol, atleta a, elemento el)
{
    int sum = sol->sum + el->grado;

    switch (el->tipo)
    {
    case NA:
        if (el->grado > a.maxNA) return 0;
        break;
    case AA:
        if (el->grado > a.maxAA) return 0;
        break;
    case AI:
        if (el->grado > a.maxAI) return 0;
        break;
    }

    if (sum > a.maxSUM)
        return 0;

    return 1;
}

int ripetuto(soluzione *sol, int pos, elemento el)
{
    int i;

    for (i = 0; i < pos; i++)
        if (sol->v[i] == el)
            return 1;

    return 0;
}

void check_best(soluzione *sol, soluzione *best)
{
    int i;

    if (sol->punteggio > best->punteggio)
    {
        best->punteggio = sol->punteggio;
        best->sum = sol->sum;
        best->flag2cat = sol->flag2cat;
        best->flaglastcat = sol->flaglastcat;

        for (i = 0; i < N_ELEMENTI; i++)
            best->v[i] = sol->v[i];
    }
}

int cat2(soluzione *sol, int pos, elemento el)
{
    int i;

    for (i = 0; i < pos; i++)
        if (sol->v[i]->tipo != el->tipo)
            return 1;

    return 0;
}

int comb_rip(int pos, int start, soluzione *sol, int n, int k, int count, tab_elementi te, atleta a, soluzione *best)
{
	int i;

	if (pos >= k)
    {
        // nel caso, copia sol in best
        check_best(sol, best);

        return count + 1;
    }

    for (i = start; i < n; i++)
    {
        // se rispetto i vincoli di difficoltà
        if (verifica(sol, a, te->elementi[i]))
        {
            int rip = ripetuto(sol, pos, te->elementi[i]);  // flag elementi ripetuti
            int flag2cat = 0;   // flag per bonus elementi di due categorie diverse
            int flag_last = 0;  // flag per bonus ultimo elemento acrobatico

            sol->v[pos] = te->elementi[i];    // lo prendo
            sol->sum += te->elementi[i]->grado;

            if (sol->flag2cat == NULL) // se non ho già preso il bonus 2cat
            {
                if (cat2(sol, pos, te->elementi[i]))  // se posso prenderlo
                {
                    sol->flag2cat = te->elementi[i];  // segno quale elemento me lo ha fatto prendere
                    flag2cat = 1;
                    sol->punteggio += BONUS;
                }
            }

            if (pos + 1 == k)   // se sto prendendo l'ultimo elemento
            {
                if (te->elementi[i]->tipo == AA || te->elementi[i]->tipo == AI) // ed è acrobatico
                {
                    if (te->elementi[i]->grado >= 5)    // di grado almeno 5
                    {
                        if (te->elementi[i] != sol->flag2cat)   // ma diverso dall'elemento del bonus precedente
                        {
                            flag_last = 1;
                            sol->flaglastcat = te->elementi[i];
                            sol->punteggio += BONUS;
                        }
                    }
                }
            }

            if (!rip)   // se non l'ho già preso
                sol->punteggio += te->elementi[i]->punti;   // conto i suoi punti

            count = comb_rip(pos + 1, start, sol, n, k, count, te, a, best);

            // BACK-TRACK
            sol->sum -= te->elementi[i]->grado;
            if (!rip)
                sol->punteggio -= te->elementi[i]->punti;
            if (flag2cat)
            {
                sol->flag2cat = NULL;
                sol->punteggio -= BONUS;
            }
            if (flag_last)
            {
                sol->flaglastcat = NULL;
                sol->punteggio -= BONUS;
            }
        }
        start++;
    }

    return count;
}

/* GREEDY */

int compare(const void *e1, const void *e2)
{
    const elemento el1 = *(const elemento *)e1;
    const elemento el2 = *(const elemento *)e2;

    float r1 = el1->punti / el1->grado;
    float r2 = el2->punti / el2->grado;

    if (r2 > r1)
        return 1;
    else if (r2 == r1)
        return 0;
    else
        return -1;
}

void greedy_aux(tab_elementi te, atleta a)
{
    int n = te->aa + te->ai + te->na;
    int pos, i;
    soluzione *sol;
    int seenNA, seenAA, seenAI;
    seenNA = seenAA = seenAI = 0;
    elemento el;
    int remaing_sum;
    int rip;
    int searcFor;   // difficoltà da cercare

    sol = malloc(sizeof(*sol));
    sol->v = malloc(N_ELEMENTI * sizeof(*(sol)->v));
    sol->sum = 0;
    sol->punteggio = 0;
    sol->flag2cat = NULL;
    sol->flaglastcat = NULL;

    remaing_sum = a.maxSUM;
    pos = 0;
    while (pos < N_ELEMENTI)
    {
        searcFor = remaing_sum / (N_ELEMENTI - pos);

        for (i = 0; i < n; i++)
        {
            if (te->elementi[i]->grado == searcFor)
            {
                el = te->elementi[i];
                remaing_sum -= searcFor;
                break;
            }
        }

        rip = ripetuto(sol, pos, el);

        switch (el->tipo)
        {
        case NA:
            if (el->grado <= a.maxNA && sol->sum + el->grado <= a.maxSUM)  // se rispetto i vincoli di difficoltà
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                if (!rip)
                    sol->punteggio += el->punti;

                if (!seenNA && !sol->flag2cat)  // se non ho ricevuto il bonus composizione
                {
                    seenNA = 1;

                    if (seenAA || seenAI)        // e se posso prenderlo
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }

                }
            }
            break;
        case AA:
            if (el->grado <= a.maxAA && sol->sum + el->grado <= a.maxSUM)
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                if (!rip)
                    sol->punteggio += el->punti;

                if (!seenAA && !sol->flag2cat)
                {
                    seenAA = 1;

                    if (seenNA || seenAI)
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }
                }
            }
            break;
        case AI:
            if (el->grado <= a.maxAI && sol->sum + el->grado <= a.maxSUM)
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                if (!rip)
                    sol->punteggio += el->punti;

                if (!seenAI && !sol->flag2cat)
                {
                    seenAI = 1;

                    if (seenNA || seenAA)
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }
                }
            }
            break;
        }
    }

    // controllo esigenza uscita
    elemento last = sol->v[pos - 1];

    if (last != sol->flag2cat)
    {
        if (last->grado >= 5 && (last->tipo == AA || last->tipo == AI))
        {
            sol->punteggio += BONUS;
            sol->flaglastcat = last;
        }
    }

    displaySol(sol);
}

void greedy(tab_elementi te, atleta a)
{
    int i, n, pos;
    soluzione *sol;
    elemento el;
    int seenNA = 0, seenAA = 0, seenAI = 0;  // flag per esigenza composizione

    n = te->aa + te->ai + te->na;
    // ordino gli elementi in base alla maggiore appetibilità (punti / grado)
    qsort(te->elementi, n, sizeof(elemento), compare);

    sol = malloc(sizeof(*sol));
    sol->v = malloc(N_ELEMENTI * sizeof(*(sol)->v));

    sol->sum = 0;
    sol->punteggio = 0;
    sol->flag2cat = NULL;
    sol->flaglastcat = NULL;


    for (i = 0, pos = 0; i < n && pos < N_ELEMENTI; i++)
    {
        el = te->elementi[i];

        switch (el->tipo)
        {
        case NA:
            if (el->grado <= a.maxNA && sol->sum + el->grado <= a.maxSUM)  // se rispetto i vincoli di difficoltà
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                sol->punteggio += el->punti;

                if (!seenNA && !sol->flag2cat)  // se non ho ricevuto il bonus composizione
                {
                    seenNA = 1;

                    if (seenAA || seenAI)        // e se posso prenderlo
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }

                }
            }
            break;
        case AA:
            if (el->grado <= a.maxAA && sol->sum + el->grado <= a.maxSUM)
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                sol->punteggio += el->punti;

                if (!seenAA && !sol->flag2cat)
                {
                    seenAA = 1;

                    if (seenNA || seenAI)
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }
                }
            }
            break;
        case AI:
            if (el->grado <= a.maxAI && sol->sum + el->grado <= a.maxSUM)
            {
                sol->v[pos++] = el;
                sol->sum += el->grado;
                sol->punteggio += el->punti;

                if (!seenAI && !sol->flag2cat)
                {
                    seenAI = 1;

                    if (seenNA || seenAA)
                    {
                        sol->flag2cat = el;
                        sol->punteggio += BONUS;
                    }
                }
            }
            break;
        }
    }

    if (pos < N_ELEMENTI)   // greedy non è riuscito a riempire sol con N_ELEMENTI compatibili con a.maxSUM
    {
        greedy_aux(te, a);
        return;
    }

    // controllo esigenza uscita
    elemento last = sol->v[pos - 1];

    if (last != sol->flag2cat)
    {
        if (last->grado >= 5 && (last->tipo == AA || last->tipo == AI))
        {
            sol->punteggio += BONUS;
            sol->flaglastcat = last;
        }
    }

    displaySol(sol);
}