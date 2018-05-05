#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>    // per abs()
#include <limits.h>  // per INT_MAX

#define MAX_STR 20

typedef struct
{
    char *nome;
    int n_abitanti;
    int dist;
} citta;

// alloca un array di città inizializzandolo con le info da file
int leggi(citta **cities, FILE *fp);
// alloca una matrice inizializzandole con le distanze mutue tra due città
void init_distance(int ***dist_city, citta *cities, int n);
// calcola la distanza tra due città, intesa come modulo della differenza
// delle due distanze dall'inizio della via
int dist2cities(citta c1, citta c2);
// sommatoria delle distanze
int somdist(citta *cities, int *ato_cities, int **dist_city, int n, int k);
// genera le combinanzioni semplici
int comb_sempl(int pos, citta *val, int **dist_city, int *sol, int *best_sol, int n, int k, int start, int count, int *best);

int main(int argc, char *argv[])
{
    FILE *fp;
    int n, i;

    citta *cities;          // vettore città
    int **dist_city;        // matrice SIMMETRICA distanze mutue
    int *sol, *best_sol;    // vettori soluzione, contengo indici del vettore cities
    int best = INT_MAX;     // minimo della sommatoria ritornato da comb_sempl
    int k = atoi(argv[2]);

    if (argc != 3)
    {
        printf("Uso: %s <file> <K>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    n = leggi(&cities, fp);
    fclose(fp);

    init_distance(&dist_city, cities, n);

    if ((sol = malloc(k * sizeof(*sol))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    if ((best_sol = malloc(k * sizeof(*sol))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    comb_sempl(0, cities, dist_city, sol, best_sol, n, k, 0, 0, &best);

    printf("best: %d\n\n", best);
    for (i = 0; i < k; i++)
        printf("%s\n", cities[best_sol[i]].nome);


    // FREE
    free(sol);
    free(best_sol);
    for (i = 0; i < n; i++)
    {
        free(cities[i].nome);
        free(dist_city[i]);
    }
    free(cities);
    free(dist_city);

    return 0;
}


int leggi(citta **cities, FILE *fp)
{
    int n, i;
    int abitanti, dist;
    char buf[MAX_STR + 1];

    fscanf(fp, "%d\n", &n);

    if ((*cities = malloc(n * sizeof(**cities))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%s %d %d", buf, &abitanti, &dist);
        (*cities)[i].nome = malloc((strlen(buf) + 1) * sizeof(*((*cities)[i].nome)));

        if ((*cities)[i].nome == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }
        strcpy((*cities)[i].nome, buf);

        (*cities)[i].n_abitanti = abitanti;
        (*cities)[i].dist       = dist;
    }

    return n;
}

void init_distance(int ***dist_city, citta *cities, int n)
{
    int i, j;

    if ((*dist_city = malloc(n * sizeof(**dist_city))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    // due for da 0 a n => algoritmo quadratico O(n^2)
    for (i = 0; i < n; i++)
    {
        if (((*dist_city)[i] = malloc(n * sizeof(***dist_city))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < n; j++)
            (*dist_city)[i][j] = dist2cities(cities[i], cities[j]);
    }
}

int dist2cities(citta c1, citta c2)
{
    return abs(c1.dist - c2.dist);
}

int somdist(citta *cities, int *ato_cities, int **dist_city, int n, int k)
{
    int i;          // contatore città
    int ato = 0;    // contatore ato
    int sum = 0;

    // ato_cities deve essere ordinato (counting sort) O(n)
    // i valori di ato_cities vanno da 0 ad n - 1, quindi
    // anche lo spazio occupato è O(n)

    // un for da 0 ad n => algoritmo lineare O(n)
    for (i = 0; i < n; i++)
    {
        // sono più vicino alla successiva ato che alla prima
        if (ato < k - 1 && dist_city[i][ato_cities[ato + 1]] < dist_city[i][ato_cities[ato]])
            ato++;

        sum += cities[i].n_abitanti * dist_city[i][ato_cities[ato]];
    }


    return sum;
}

int comb_sempl(int pos, citta *val, int **dist_city, int *sol, int *best_sol, int n, int k, int start, int count, int *best)
{
    int i;

    if (pos >= k)
    {
        int cur_dist = somdist(val, sol, dist_city, n, k);

        if (cur_dist < *best)
        {
            *best = cur_dist;

            for (i = 0; i < k; i++)
                best_sol[i] = sol[i];
        }

        return count + 1;
    }

    for (i = start; i < n; i++)
    {
        sol[pos] = i;
        count = comb_sempl(pos + 1, val, dist_city, sol, best_sol, n, k, i + 1, count, best);
    }

    return count;
}
