#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
} Masina;

typedef struct Nod {
    Masina info;
    struct Nod* next;
} Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";

    if (!fgets(buffer, 100, file)) {
        Masina m;
        m.id = 0;
        return m;
    }

    Masina m;
    char* aux;

    aux = strtok(buffer, sep);
    if (aux == NULL) {
        m.id = 0;
        return m;
    }

    m.id = atoi(aux);
    m.nrUsi = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m.model = malloc(strlen(aux) + 1);
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m.numeSofer, aux);

    m.serie = *strtok(NULL, sep);

    return m;
}

void afisareMasina(Masina m) {
    printf("Id: %d\n", m.id);
    printf("Nr usi: %d\n", m.nrUsi);
    printf("Pret: %.2f\n", m.pret);
    printf("Model: %s\n", m.model);
    printf("Nume sofer: %s\n", m.numeSofer);
    printf("Serie: %c\n\n", m.serie);
}

void adaugaMasinaInLista(Nod** lista, Masina m) {
    Nod* nou = malloc(sizeof(Nod));
    nou->info = m;
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
    }
    else {
        Nod* p = *lista;
        while (p->next)
            p = p->next;
        p->next = nou;
    }
}

void adaugaLaInceputInLista(Nod** lista, Masina m) {
    Nod* nou = malloc(sizeof(Nod));
    nou->info = m;
    nou->next = *lista;
    *lista = nou;
}

void afisareListaMasini(Nod* lista) {
    while (lista) {
        afisareMasina(lista->info);
        lista = lista->next;
    }
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Nod* lista = NULL;

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);

        if (m.id != 0) {   
            adaugaMasinaInLista(&lista, m);
        }
    }

    fclose(f);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    while (*lista) {
        free((*lista)->info.model);
        free((*lista)->info.numeSofer);

        Nod* aux = *lista;
        *lista = (*lista)->next;
        free(aux);
    }
}

float calculeazaPretMediu(Nod* lista) {
    float suma = 0;
    int nr = 0;

    while (lista) {
        suma += lista->info.pret;
        nr++;
        lista = lista->next;
    }

    if (nr == 0) return 0;
    return suma / nr;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
    while (*lista && (*lista)->info.serie == serieCautata) {
        Nod* aux = *lista;
        *lista = (*lista)->next;

        free(aux->info.model);
        free(aux->info.numeSofer);
        free(aux);
    }

    Nod* p = *lista;
    while (p && p->next) {
        if (p->next->info.serie == serieCautata) {
            Nod* aux = p->next;
            p->next = aux->next;

            free(aux->info.model);
            free(aux->info.numeSofer);
            free(aux);
        }
        else {
            p = p->next;
        }
    }
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
    float suma = 0;

    while (lista) {
        if (strcmp(lista->info.numeSofer, numeSofer) == 0)
            suma += lista->info.pret;
        lista = lista->next;
    }

    return suma;
}

int main() {

    Nod* lista = citireListaMasiniDinFisier("masini2.txt");

    printf("Lista citita:\n");
    afisareListaMasini(lista);

    printf("%.2f\n\n", calculeazaPretMediu(lista));

    printf("%.2f\n\n", calculeazaPretulMasinilorUnuiSofer(lista, "Ion"));

    stergeMasiniDinSeria(&lista, 'A');
    afisareListaMasini(lista);

    dezalocareListaMasini(&lista);

    return 0;
}
