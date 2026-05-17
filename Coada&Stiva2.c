#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Masina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};

struct Nod {
    Masina info;
    Nod* next;
};

Masina initMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
    Masina m;
    m.id = id;
    m.nrUsi = nrUsi;
    m.pret = pret;

    m.model = (char*)malloc(strlen(model) + 1);
    strcpy(m.model, model);

    m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
    strcpy(m.numeSofer, numeSofer);

    m.serie = serie;

    return m;
}

void afisareMasina(Masina m) {
    printf("ID: %d | Usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
        m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

Masina citireMasinaDinFisier(FILE* f) {
    char buffer[128];
    char sep[3] = ",\n";

    if (!fgets(buffer, 128, f))
        return initMasina(-1, 0, 0, "", "", 0);

    char* token = strtok(buffer, sep);
    int id = atoi(token);

    int nrUsi = atoi(strtok(NULL, sep));
    float pret = atof(strtok(NULL, sep));

    char* model = strtok(NULL, sep);
    char* sofer = strtok(NULL, sep);

    unsigned char serie = *strtok(NULL, sep);

    return initMasina(id, nrUsi, pret, model, sofer, serie);
}

void push(Nod** varf, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = m;
    nou->next = *varf;
    *varf = nou;
}

Masina pop(Nod** varf) {
    if (*varf == NULL)
        return initMasina(-1, 0, 0, "", "", 0);

    Nod* aux = *varf;
    Masina m = aux->info;

    *varf = aux->next;
    free(aux);

    return m;
}

void put(Nod** coada, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = m;
    nou->next = NULL;

    if (*coada == NULL) {
        *coada = nou;
    }
    else {
        Nod* aux = *coada;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = nou;
    }
}

Masina get(Nod** coada) {
    if (*coada == NULL)
        return initMasina(-1, 0, 0, "", "", 0);

    Nod* aux = *coada;
    Masina m = aux->info;

    *coada = aux->next;
    free(aux);

    return m;
}

Nod* citireStackDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Nod* stiva = NULL;

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        if (m.id != -1)
            push(&stiva, m);
    }

    fclose(f);
    return stiva;
}

Nod* citireCoadaDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Nod* coada = NULL;

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        if (m.id != -1)
            put(&coada, m);
    }

    fclose(f);
    return coada;
}

void dezalocareMasina(Masina m) {
    free(m.model);
    free(m.numeSofer);
}

int main() {

   
    Nod* stiva = citireStackDinFisier("masini2.txt");

    while (stiva != NULL) {
        Masina m = pop(&stiva);
        afisareMasina(m);
        dezalocareMasina(m);
    }

    Nod* coada = citireCoadaDinFisier("masini2.txt");

    while (coada != NULL) {
        Masina m = get(&coada);
        afisareMasina(m);
        dezalocareMasina(m);
    }

    return 0;
}
