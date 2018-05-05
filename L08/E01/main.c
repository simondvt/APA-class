#include <stdio.h>
#include <stdlib.h>

#define HAB "habitat.txt"
#define FAM "famiglie.txt"
#define NEM "nemici.txt"

typedef struct
{
    int *eucalipti;  // vettore di eucalipti su cui vivere
    int n_eucalipti; // lunghezza del vettore eucalipti
    int id_famiglia; // identificativo famiglia
} koala;

// alloca k e legge i koala, determina anche il numero di eucalipti
int leggiHab(koala **k, int *tot_eucalipti);
// legge le famiglie
int leggiFam(koala *k);
// alloca e legge le incompatibilità tra famiglie salvandole in una matrice simmetrica
void leggiNem(int ***nem, int n_fam);
// calcola e visualizza le partizioni
void disp_rip(int pos, int *val, int *sol, int n, int k, int **nem, koala *koali, int max_koala);
// controlla i vincoli (NO VINCOLI HAB)
int verifySol(int *sol, int eucalipti, int pos, int **nem, koala *k, int max_koala);
// controlla i vincoli
int verifySolHab(int *sol, int eucalipti, int pos, int **nem, koala *k, int max_koala);

int main(int argc, char *argv[])
{
    koala *k;          // vettore di struct
    int **nem;         // matrice simmetrica, tiene conto delle incompatibilità
    int n_koala;       // numero di koala
    int n_fam;         // numero di famiglie
    int max_koala;     // numero massimo di koala che possono vivere su un eucalipto
    int tot_eucalipti; // numero di eucalipti
	int i;

    if (argc != 2)
    {
        printf("Uso: <%s> m\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n_koala = leggiHab(&k, &tot_eucalipti);
    n_fam = leggiFam(k);
    leggiNem(&nem, n_fam);
    max_koala = atoi(argv[1]);

    if (max_koala * tot_eucalipti < n_koala)
    {
        printf("Nella foresta non c'è posto per tutti i koala!\n");
        exit(EXIT_FAILURE);
    }

    int *sol, *val;
    sol = malloc(n_koala * sizeof(int));
    val = malloc(n_koala * sizeof(int));
	for (i = 0; i < n_koala; i++)
	{
		val[i] = i;
		sol[i] = -1;
	}

	printf("Ci sono %d eucalipti disponibili\n\n", tot_eucalipti);
	printf("KOALA:\n");
	for (i = 0; i < n_koala; i++)
    {
        printf("-Koala %d appartiene alla famiglia %d\n", i, k[i].id_famiglia);
        printf("Koala %d puo' stare sugli eucalipti:\n", i);
        int j;
        for (j = 0; j < k[i].n_eucalipti; j++)
        {
            printf("%d\t", k[i].eucalipti[j]);
        }
        printf("\n\n");
    }

    for (i = 0; i < n_fam; i++)
    {
        int j;
        for (j = n_fam - 1; j > i; j--)
        {
            if (nem[i][j] == 1)
                printf("*Famiglie %d e %d sono nemiche\n\n", i, j);
        }
    }


    disp_rip(0, val, sol, n_koala, tot_eucalipti + 1, nem, k, max_koala);

    return 0;
}

int leggiHab(koala **k, int *tot_eucalipti)
{
    FILE *fp;
    int n, n_eucalipti;
    int i, j;

    if ((fp = fopen(HAB, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", HAB);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d\n", &n);
    if ((*k = malloc(n * sizeof(**k))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    *tot_eucalipti = 0;
    // per ogni koala
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d\n", &n_eucalipti);
        (*k)[i].n_eucalipti = n_eucalipti;

        if (((*k)[i].eucalipti = malloc(n_eucalipti * sizeof(*((*k)[i].eucalipti)))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }

        // per ogni eucalipto di un koala
        for (j = 0; j < n_eucalipti; j++)
        {
            fscanf(fp, "%d\n", &((*k)[i].eucalipti[j]));

            // trovo il numero totale di eucalipti
            if ((*k)[i].eucalipti[j] > *tot_eucalipti)
                *tot_eucalipti = (*k)[i].eucalipti[j];
        }

    }

    fclose(fp);
    return n;
}

int leggiFam(koala *k)
{
    FILE *fp;
    int n, quanti, quale;
    int i, j;

    if ((fp = fopen(FAM, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", FAM);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d\n", &n);

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d\n", &quanti);

        for (j = 0; j < quanti; j++)
        {
            fscanf(fp, "%d\n", &quale);
            k[quale].id_famiglia = i;
        }

    }

    fclose(fp);
    return n;
}

void leggiNem(int ***nem, int n_fam)
{
    FILE *fp;
    int p, q;
    int i;

    if ((fp = fopen(NEM, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", NEM);
        exit(EXIT_FAILURE);
    }

    if ((*nem = malloc(n_fam * sizeof(**nem))) == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }

    // inizialmente nessuna famiglia è in conflitto
    for (i = 0; i < n_fam; i++)
    {
        if (((*nem)[i] = calloc(n_fam, sizeof(***nem))) == NULL)
        {
            printf("Memoria insufficiente\n");
            exit(EXIT_FAILURE);
        }
    }

    // setto a 1 le famiglie in conflitto
    while (fscanf(fp, "%d %d\n", &p, &q) != EOF)
        (*nem)[p][q] = (*nem)[q][p] = 1;


    fclose(fp);
}

int verifySol(int *sol, int eucalipti, int pos, int **nem, koala *k, int max_koala)
{
    int i, j, l;
    int fam1, fam2;
    int num_koala;

    for (i = 0; i < eucalipti; i++)
    {
        // controllo il vettore verificando i vari accoppiamenti
        for (j = 0; j < pos; j++)
        {
            num_koala = 0;
            for (l = j; l < pos && sol[j] == i; l++)
            {
                if (sol[l] == i)
                {
                    num_koala++;

                    // controllo numero max di koala su un eucalipto
                    if (num_koala > max_koala)
                        return 0;

                    // controllo incompatibilità famiglie
                    fam1 = k[j].id_famiglia;
                    fam2 = k[l].id_famiglia;

                    if (nem[fam1][fam2] == 1)
                        return 0;
                }
            }
        }
    }

    return 1;
}

int verifySolHab(int *sol, int eucalipti, int pos, int **nem, koala *k, int max_koala)
{
    int i, j, l, h;
    int trovato;
    int fam1, fam2;
    int num_koala;

    for (i = 0; i < eucalipti; i++)
    {
        for (j = 0; j < pos; j++)
        {
            num_koala = 0;
            for (l = j; l < pos && sol[j] == i; l++)
            {
                if (sol[l] == i)
                {
                    // controllo se il koala l può stare sull eucalitpo i
                    trovato = 0;
                    for (h = 0; h < k[l].n_eucalipti; h++)
                        if (i == k[l].eucalipti[h])
                            trovato = 1;
                    if (trovato == 0)
                        return 0;
                    // fine controllo

                    num_koala++;

                    if (num_koala > max_koala)
                        return 0;

                    fam1 = k[j].id_famiglia;
                    fam2 = k[l].id_famiglia;

                    if (nem[fam1][fam2] == 1)
                        return 0;
                }
            }
        }
    }

    return 1;
}

void disp_rip(int pos, int *val, int *sol, int n, int k, int **nem, koala *koali, int max_koala)
{
	int i, j;
	int ok = 1;
	int *occ;

	if (pos >= n)
	{
		occ = calloc(k, sizeof(int));
		for (i = 0; i < n; i++)
			occ[sol[i]]++;

		i = 0;
		while (i < k && ok)
		{
			if (occ[i] == 0)
				ok = 0;

			i++;
		}

		free(occ);

        if (ok == 0)
            return;

        if (verifySolHab(sol, k, pos, nem, koali, max_koala))
        {
            for (i = 0; i < k; i++)
            {
                printf("Eucalipto %d:\n{ ", i);
                for (j = 0; j < n; j++)
                {
                    if (sol[j] == i)
                        printf("%d ", val[j]);
                }

                printf("}\n");
            }

            printf("\n");
        }

        return;
	}

	for (i = 0; i < k; i++)
	{
		sol[pos] = i;
		//if (verifySolHab(sol, k, pos, nem, koali, max_koala))
            disp_rip(pos + 1, val, sol, n, k, nem, koali, max_koala);
	}
}
