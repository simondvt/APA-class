#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRCFILE "atleti.txt"
#define MAXSTR 25

typedef struct
{
    char *id;
    char *nome;
    char *cognome;
    char *categoria;
    char *data;
    int ore;
} Atleta;

typedef struct node
{
    Atleta val;
    struct node *next;
} node_t, *link;


// creazione di un nuovo nodo
// lista nuova ritornata come valore di ritorno
link newNode(Atleta val, link next);

// inserimento in coda con passaggio per riferimento
void listInsTailP(link *hp, Atleta val);

// aggiunge in nuovo atleta come ultimo in lista
void addAtleta(link *hp);

// cancella un atleta a partire dall ID
void deleteAtleta(link *hp);

// legge i dati da file memorizzandoli nella lista head
// ritorna il numero n di atleti letti
int leggi(link *head);

// stampa (a video o file) i contenuti dell'anagrafica
void stampa(link head);

// funzione per la qsort
int lessCategoria(const void *a1, const void *a2);

// stampa gli atleti divisi per categoria
void stampaCategoria(link head, int n);

// stampa un singolo atleta sullo stream out
void stampaAtleta(Atleta atleta, FILE *out);

// ricerca di un atleta per codice
void codeSearch(link head);

// ricerca di un atleta per cognome
void nameSearch(link head);

// ricerca l'atleta e poi modifica il suo monte ore
void aggiornaOre(link head);

// libera la memoria
void destroy(link head);



// legge i dati da file e si interfaccia con l'utente
// mediante un menu basato su intero
int main(void)
{
    link head;
    int n;      // numero di atleti letti
    int scelta; // scelta del menu

    n = leggi(&head);

    printf("Letti %d atleti con successo.\n", n);
    printf("Scegliere opzione:\n");
    printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n");
    scanf("%d", &scelta);
    do
    {
        fflush(stdin);


        switch (scelta)
        {
            case 0: destroy(head);                          return 0;
            case 1: stampa(head);                           break;
            case 2: stampaCategoria(head, n);               break;
            case 3: aggiornaOre(head);                      break;
            case 4: codeSearch(head);                       break;
            case 5: nameSearch(head);                       break;
            case 6: addAtleta(&head);       n++;            break;
            case 7: deleteAtleta(&head);    n--;            break;

            default: printf("Scelta non valida!\n\n");
        }

        printf("0 - Esci\n1 - Stampa\n2 - Stampa per categoria\n3 - Aggiornamento ore settimanali\n4 - Ricerca per codice\n5 - Ricerca per cognome\n6 - Aggiungi nuovo atleta\n7 - Cancella atleta\n");
        scanf("%d", &scelta);
    } while (scelta != 0);

    return 0;
}


link newNode(Atleta val, link next)
{
    link x = malloc(sizeof(*x));

    if (x == NULL)
        return NULL;

    x->val = val;
    x->next = next;

    return x;
}

void listInsTailP(link *hp, Atleta val)
{
    link x = *hp;

    if (x == NULL)
        *hp = newNode(val, NULL);
    else
    {
        for (; x->next != NULL; x = x->next);
        x->next = newNode(val, NULL);
    }
}


void addAtleta(link *hp)
{
    int dd, mm, yy;
    char buf[MAXSTR + 1];
    Atleta val;

    printf("Inserire ID: ");
    fscanf(stdin, "%s", buf);
    val.id = malloc((strlen(buf) + 1) * sizeof(*(val.id)));
    strcpy(val.id, buf);

    printf("Inserire nome: ");
    fscanf(stdin, "%s", buf);
    val.nome = malloc((strlen(buf) + 1) * sizeof(*(val.nome)));
    strcpy(val.nome, buf);

    printf("Inserire cognome: ");
    fscanf(stdin, "%s", buf);
    val.cognome = malloc((strlen(buf) + 1) * sizeof(*(val.cognome)));
    strcpy(val.cognome, buf);

    printf("Inserire categoria: ");
    fscanf(stdin, "%s", buf);
    val.categoria = malloc((strlen(buf) + 1) * sizeof(*(val.categoria)));
    strcpy(val.categoria, buf);

    printf("Inserire data (gg/mm/aaaa): ");
    fscanf(stdin, "%2d/%2d/%4d", &dd, &mm, &yy);
    val.data = malloc((10 + 1) * sizeof(*(val.data))); // dd/mm/yyyy sono 10 caratteri
    sprintf(val.data, "%02d/%02d/%4d", dd, mm, yy);

    printf("Inserire ore: ");
    fscanf(stdin, "%d", &(val.ore));

    listInsTailP(hp, val);
}

void deleteAtleta(link *hp)
{
    link x, p;
    char code[MAXSTR + 1];

    printf("Inserire ID dell atleta da cancellare: ");
    scanf("%s", code);

    if (*hp == NULL)
        return;

    for (x = *hp, p = NULL; x != NULL; p = x, x = x->next)
    {
        if (strcmp(code, x->val.id) == 0)
        {
            if (x == *hp)
                *hp = x->next;
            else
                p->next = x->next;

            free(x->val.categoria);
            free(x->val.nome);
            free(x->val.cognome);
            free(x->val.data);
            free(x->val.id);
            free(x);
            break;
        }
    }
}


int leggi(link *head)
{
    Atleta val;
    char buf[MAXSTR + 1];
    int i, n;
    FILE *fp;
    int dd, mm, yy;

    if ((fp = fopen(SRCFILE, "r")) == NULL)
    {
        printf("Impossibile aprire %s\n", SRCFILE);
        exit(1);
    }

    fscanf(fp, "%d", &n);

    *head = NULL;
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%s", buf);
        val.id = malloc((strlen(buf) + 1) * sizeof(*(val.id)));
        strcpy(val.id, buf);

        fscanf(fp, "%s", buf);
        val.nome = malloc((strlen(buf) + 1) * sizeof(*(val.nome)));
        strcpy(val.nome, buf);

        fscanf(fp, "%s", buf);
        val.cognome = malloc((strlen(buf) + 1) * sizeof(*(val.cognome)));
        strcpy(val.cognome, buf);

        fscanf(fp, "%s", buf);
        val.categoria = malloc((strlen(buf) + 1) * sizeof(*(val.categoria)));
        strcpy(val.categoria, buf);

        fscanf(fp, "%2d/%2d/%4d", &dd, &mm, &yy);
        val.data = malloc((10 + 1) * sizeof(*(val.data))); // dd/mm/yyyy sono 10 caratteri
        sprintf(val.data, "%02d/%02d/%4d", dd, mm, yy);

        fscanf(fp, "%d", &(val.ore));

        listInsTailP(head, val);
    }

    fclose(fp);
    return n;
}


void stampa(link head)
{
    FILE *out;
    char sOut[MAXSTR + 1];

    printf("Inserire lo stream su cui stampare i dati(vuoto per stdout): ");
    fgets(sOut, MAXSTR, stdin);

    if (sOut[0] == '\n')  // se l'utente lascia vuoto la fgets salva su sOut "\n"
        out = stdout;
    else
    {
        // fopen non può aprire nomi di file che terminano in \n, quindi tronco in quel punto
        sOut[strlen(sOut) - 1] = '\0';

        if ((out = fopen(sOut, "w")) == NULL)
        {
            printf("Impossibile scrivere su %s\n", sOut);
            exit(1);
        }
    }


    for (; head != NULL; head = head->next)
        stampaAtleta(head->val, out);


    if (out != stdout)
        fclose(out);
}


int lessCategoria(const void *a1, const void *a2)
{
    // a2 e a2 sono di tipo Atleta ** perché così richiede la qsort
    Atleta **atleta1 = (Atleta **) a1;
    Atleta **atleta2 = (Atleta **) a2;

    return strcmp((*atleta1)->categoria, (*atleta2)->categoria);
}

void stampaCategoria(link head, int n)
{
    int i;
    char categoria[MAXSTR + 1];

    // atleti è un vettore di puntatori a struct perché così non spreco memoria e spazio
    // a ricopiare le struct, ma copio solo i puntatori
    Atleta **atleti = malloc(n * sizeof(*atleti));

    if (atleti == NULL)
    {
        printf("Memoria insufficiente\n");
        exit(1);
    }

    // inizializzo l'array, ogni elemento contiene una struttura atleti
    for (i = 0; i < n; i++, head = head->next)
        atleti[i] = &(head->val);


    // ordino
    qsort(atleti, n, sizeof(*atleti), lessCategoria);

    strcpy(categoria, atleti[0]->categoria);
    printf("Categoria [%s]:\n", categoria);

    for (i = 0; i < n; i++)
    {
        // se sono passato ad una nuova categoria, stampo il relativo nome
        if (strcmp(atleti[i]->categoria, categoria) != 0)
        {
            strcpy(categoria, atleti[i]->categoria);
            printf("\nCategoria [%s]:\n", categoria);
        }

        stampaAtleta(*atleti[i], stdout);
    }

    free(atleti);
}


void stampaAtleta(Atleta atleti, FILE *out)
{
    fprintf(out, "ID: %s\n", atleti.id);
    fprintf(out, "Nome: %s\n", atleti.nome);
    fprintf(out, "Cognome: %s\n", atleti.cognome);
    fprintf(out, "Categoria: %s\n", atleti.categoria);
    fprintf(out, "Data: %s\n", atleti.data);
    fprintf(out, "Ore: %d\n\n", atleti.ore);
}


void codeSearch(link head)
{
    char code[MAXSTR + 1];

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    for (; head != NULL; head = head->next)
    {
        if (strcmp(code, head->val.id) == 0)
        {
            stampaAtleta(head->val, stdout);
            return;
        }
    }

    printf("Atleta non trovato!\n\n");
}


void nameSearch(link head)
{
    char name[MAXSTR + 1];

    printf("Inserire il nome dell' atleta: ");
    scanf("%s", name);

    for (; head != NULL; head = head->next)
    {
        // uso di strncmp per la ricerca parziale
        if (strncmp(name, head->val.cognome, strlen(name)) == 0)
        {
            stampaAtleta(head->val, stdout);
            return;
        }
    }

    printf("Atleta non trovato!\n\n");
}


void aggiornaOre(link head)
{
    char code[MAXSTR + 1];
    int ore;

    printf("Inserire codice atleta: ");
    scanf("%s", code);

    for (; head != NULL; head = head->next)
    {
        if (strcmp(code, head->val.id) == 0) // trovato
        {
            stampaAtleta(head->val, stdout);

            printf("Inserire ore: ");
            scanf("%d", &ore);
            head->val.ore = ore;

            break;
        }
    }
}

// comincio a distruggere dall'ultimo usando la ricorsione
// cosi non perdo il riferimento al precedente
void destroy(link head)
{
    if (head == NULL)
        return;

    destroy(head->next);

    free(head->val.id);
    free(head->val.data);
    free(head->val.cognome);
    free(head->val.nome);
    free(head->val.categoria);
    free(head);
}
