#include <stdio.h>
#include <stdlib.h>
#include "target.h"
#include "risorsa.h"

typedef enum {NULLA, RISORSA, TARGET} casellaT;

typedef struct
{
    casellaT tipo_casella;  // etichetta che mi dice se nella casella c'è una risorsa o un target
    Risorsa risorsa;
    Target target;

    int attack;     // somma degli attacchi e difese di tutte le risorse
    int defense;    // il cui raggio comprende questa cella
} Casella;

// legge da file un array di risorse
int leggiRisorse(Risorsa **risorse, FILE *fp);
// legge da file una lista di targets
Target leggiTargets(FILE *fp);
// dispozioni ripetute delle n + 1 risorse di classe R * C
int disp_rip(int pos, Risorsa *val, int R, int C, int n, int k, int budget, Casella **map, Target headTarget);

int main(int argc, char *argv[])
{
    Risorsa *risorse;           // vettore di puntatori alle risorse
    Target headTarget = NULL;   // lista di target
    Target cur;
    Casella **map;              // matrice di caselle
    FILE *fp;
    int n;                      // numero di risorse
    int budget, R, C;
    int i, j;

    if (argc != 3)
    {
        printf("Uso: %s <file_risorse> <file_target>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* LETTURA RISORSE */
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    n = leggiRisorse(&risorse, fp);
    fclose(fp);


    /* LETTURA TARGET */
    if ((fp = fopen(argv[2], "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    headTarget = leggiTargets(fp);
    fclose(fp);

    /* LETTURA R, C, budget */
    printf("Inserire R e C: ");
    scanf("%d %d", &R, &C);
    printf("Inserire budget: ");
    scanf("%d", &budget);

    /* CREO LA MAPPA E CI POSIZIONO I TARGET */
    map = malloc(R * sizeof(*map));
    for (i = 0; i < R; i++)
    {
        map[i] = malloc(C * sizeof(**map));

        for (j = 0; j < C; j++)
        {
            map[i][j].attack = 0;
            map[i][j].defense = 0;
            map[i][j].tipo_casella = NULLA;
        }
    }

    for (cur = headTarget; cur != NULL; cur = cur->next)
    {
        map[cur->r][cur->c].tipo_casella = TARGET;
        map[cur->r][cur->c].target = cur;
    }


    if (disp_rip(0, risorse, R, C, n, R * C, budget, map, headTarget))
    {
        printf("Dispozione trovata:\n");

        for (i = 0; i < R; i++)
        {
            for (j = 0; j < C; j++)
            {
                if (map[i][j].tipo_casella == RISORSA && map[i][j].risorsa->c != 0)
                {
                    printf("[%d][%d] r = %d a = %d d = %d c = %d\n", i, j, map[i][j].risorsa->r,
                           map[i][j].risorsa->a, map[i][j].risorsa->d, map[i][j].risorsa->c);
                }
            }
        }
    }
    else
        printf("Disposizione NON trovata...\n");

    return 0;
}

int leggiRisorse(Risorsa **risorse, FILE *fp)
{
    int i, n;

    fscanf(fp, "%d\n", &n);
    *risorse = malloc((n + 1) * sizeof(**risorse)); // n + 1 per la risorsa nulla

    for (i = 0; i < n; i++)
        (*risorse)[i] = leggiRisorsa(fp);
    (*risorse)[n] = newRisorsa(0, 0, 0, 0);

    return n + 1;
}

Target leggiTargets(FILE *fp)
{
    Target headTarget, cur = NULL;
    while ((cur = insTarget(cur, fp)) != NULL)
        headTarget = cur;

    return headTarget;
}

int valida(Casella **map, Target headTarget)
{
    Target cur;

    for (cur = headTarget; cur != NULL; cur = cur->next)
    {
        if (cur->tipo_target == ALLEATO)
        {
            if (map[cur->r][cur->c].defense < cur->v)
                return 0;
        }
        else    /* NEMICO */
        {
            if (map[cur->r][cur->c].attack < cur->v)
                return 0;
        }
    }

    return 1;
}

int disp_rip(int pos, Risorsa *val, int R, int C, int n, int k, int budget, Casella **map, Target headTarget)
{
    int i;
    int r, c;
    int x, y;

    /* per backtrack */
    casellaT oldT;

    if (budget < 0)
        return 0;

	if (pos >= k)
	{
	    if (valida(map, headTarget))
            return 1;

		return 0;
	}

	for (i = 0; i < n; i++)
	{
	    r = pos / C;
	    c = pos % C;

	    // se la casella non è libera posso piazzare solo la risorsa nulla
        if ((map[r][c].tipo_casella != NULLA) && val[i]->c != 0)
            continue;

	    oldT = map[r][c].tipo_casella;

	    map[r][c].tipo_casella = RISORSA;
	    map[r][c].risorsa = val[i];
	    for (x = r - val[i]->r; x <= r + val[i]->r; x++)
        {
            for (y = c - val[i]->r; y <= c + val[i]->r; y++)
            {
                if (x >= 0 && x < R && y >= 0 && y < C)
                {
                    map[x][y].attack += val[i]->a;
                    map[x][y].defense += val[i]->d;
                }
            }
        }


		if (disp_rip(pos + 1, val, R, C, n, k, budget - val[i]->c, map, headTarget))
            return 1;

        // backtrack
        map[r][c].tipo_casella = oldT;
	    for (x = r - val[i]->r; x <= r + val[i]->r; x++)
        {
            for (y = c - val[i]->r; y <= c + val[i]->r; y++)
            {
                if (x >= 0 && x < R && y >= 0 && y < C)
                {
                    map[x][y].attack -= val[i]->a;
                    map[x][y].defense -= val[i]->d;
                }
            }
        }
	}

	return 0;
}