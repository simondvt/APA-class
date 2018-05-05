#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VOLI "voli.txt"
#define MAX_VOLI 1000
#define STRLEN (30 + 1)

// strutture dati
    typedef enum
    {
        r_date, r_origine, r_destinazione, r_vettore, r_fine
    } comando_e;

    typedef struct
    {
        char id[STRLEN];
        char partenza[STRLEN];
        char destinazione[STRLEN];
        char data[STRLEN];
        char vettore[STRLEN];
    } Voli;
// end-strutture dati


// prototipi
    void strLower(char *s);
    comando_e leggiComando(void);
    int leggiTabella(Voli volo[], int size);
    void selezionaDati(Voli volo[], int size, comando_e cmd);

    void Rdate(Voli volo[], int size, char *cmd);
    void Rorigine(Voli volo[], int size, char *cmd);
    void Rdestinazione(Voli volo[], int size, char *cmd);
    void Rvettore(Voli volo[], int size, char *cmd);
// end-prototipi


// main
int main(void)
{
    Voli volo[MAX_VOLI];
    int size;       // numero effettivo di voli
    comando_e cmd;

    size = leggiTabella(volo, MAX_VOLI);

    for (;;)
    {
        cmd = leggiComando();
        selezionaDati(volo, size, cmd);
    }

    // l'uscita la gestisco in selezionaDati
}


// implementazione funzioni
void strLower(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
}

comando_e leggiComando(void)
{
    char cmd[STRLEN];
    comando_e c = 0;
    char *lista[] = {"date", "origine", "destinazione", "vettore", "fine"};
    int corretto;

    do
    {
        corretto = 1;
        printf("Inserire comando (date/origine/destinazione/vettore/fine): ");
        scanf("%s", cmd);
        strLower(cmd);

        while (c < r_fine && strcmp(cmd, lista[c]) != 0)
            c++;

        if (c > r_fine)
        {
            corretto = 0;

            // la stringa dopo il comando errato rimane nel buffer
            fflush(stdin);
        }


        if (!corretto)
            printf("\nComando Errato!\n");

    } while (!corretto);


    return c;
}

int leggiTabella(Voli volo[], int size)
{
    int realSize;
    int i;

    FILE *src;

    if ((src = fopen(VOLI, "r")) == NULL)
    {
        printf("Impossibile aprire %s.\n", VOLI);
        exit(1);
    }

    fscanf(src, "%d", &realSize);

    if (realSize > MAX_VOLI)
    {
        printf("Spazio insufficiente.\n");
        exit(1);
    }

    for (i = 0; i < realSize; i++)
    {
        fscanf(src, "%s %s %s %s %s",
               volo[i].id,
               volo[i].partenza,
               volo[i].destinazione,
               volo[i].data,
               volo[i].vettore);
    }

    fclose(src);
    return realSize;
}


void selezionaDati(Voli volo[], int size, comando_e cmd)
{
    char input[STRLEN];

    fgets(input, STRLEN, stdin);

    switch (cmd)
    {
        // (input + 1) perché salto lo spazio iniziale lasciato dalla prima scanf
        case r_date:         Rdate(volo, size, (input + 1));         break;
        case r_origine:      Rorigine(volo, size, (input + 1));      break;
        case r_destinazione: Rdestinazione(volo, size, (input + 1)); break;
        case r_vettore:      Rvettore(volo, size, (input + 1));      break;
        case r_fine: exit(0);
        // non necessito il default per comandi errati perché già gestito in leggiComando
    }
}


void Rdate(Voli volo[], int size, char *cmd)
{
    int i;
    int g1, g2, m1, m2, a1, a2;
    char data1[STRLEN], data2[STRLEN];

    sscanf(cmd, "%4d/%2d/%2d%4d/%2d/%2d", &a1, &m1, &g1, &a2, &m2, &g2);
    sprintf(data1, "%04d/%02d/%02d", a1, m1, g1);
    sprintf(data2, "%04d/%02d/%02d", a2, m2, g2);

    for (i = 0; i < size; i++)
    {
        // data1 < volo[i].data < data2
        if (strcmp(volo[i].data, data1) > 0 &&  strcmp(volo[i].data, data2) < 0)
            printf("%s\n", volo[i].id);
    }
}


void Rorigine(Voli volo[], int size, char *cmd)
{
    int i;

    for (i = 0; i < size; i++)
    {
        // in cmd rimane il \n alla fine e quindi le stringhe non sarebbero mai uguali
        // per questo uso strncmp
        if (strncmp(volo[i].partenza, cmd, strlen(volo[i].partenza)) == 0)
            printf("%s\n", volo[i].id);
    }
}


void Rdestinazione(Voli volo[], int size, char *cmd)
{
    int i;

    for (i = 0; i < size; i++)
    {
        // in cmd rimane il \n alla fine e quindi le stringhe non sarebbero mai uguali
        // per questo uso strncmp
        if (strncmp(volo[i].destinazione, cmd, strlen(volo[i].destinazione)) == 0)
            printf("%s\n", volo[i].id);
    }
}


void Rvettore(Voli volo[], int size, char *cmd)
{
    int i;

    for (i = 0; i < size; i++)
    {
        // in cmd rimane il \n alla fine e quindi le stringhe non sarebbero mai uguali
        // per questo uso strncmp
        if (strncmp(volo[i].vettore, cmd, strlen(volo[i].vettore)) == 0)
            printf("%s\n", volo[i].partenza); // <-- l'esempio stampa la partenza
    }
}
