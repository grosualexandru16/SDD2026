#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

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

struct HashTable {
    int dim;
    Nod** vector; 
};

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";

    Masina m;

    if (!fgets(buffer, 100, file)) {
        m.id = -1;
        return m;
    }

    char* aux = strtok(buffer, sep);
    m.id = atoi(aux);

    m.nrUsi = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m.numeSofer, aux);

    m.serie = *strtok(NULL, sep);

    return m;
}

void afisareMasina(Masina m) {
    printf("ID: %d | Usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
        m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void afisareListaMasini(Nod* lista) {
    while (lista != NULL) {
        afisareMasina(lista->info);
        lista = lista->next;
    }
}

void adaugaMasinaInLista(Nod** lista, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = m;
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
    }
    else {
        Nod* aux = *lista;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = nou;
    }
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);

    for (int i = 0; i < dim; i++)
        ht.vector[i] = NULL;

    return ht;
}

int calculeazaHash(int id, int dim) {
    return id % dim;
}

void inserareMasinaInTabela(HashTable ht, Masina m) {
    int poz = calculeazaHash(m.id, ht.dim);
    adaugaMasinaInLista(&ht.vector[poz], m);
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
   

    HashTable ht = initializareHashTable(10);

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        if (m.id != -1)
            inserareMasinaInTabela(ht, m);
    }

    fclose(f);
    return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        printf("Cluster %d:\n", i);
        afisareListaMasini(ht.vector[i]);
        printf("\n");
    }
}

void dezalocareTabelaDeMasini(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* aux = ht->vector[i];
        while (aux != NULL) {
            Nod* temp = aux;
            aux = aux->next;

            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
        }
    }
    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

Masina getMasinaDupaCheie(HashTable ht, int idCautat) {
    int poz = calculeazaHash(idCautat, ht.dim);
    Nod* aux = ht.vector[poz];

    while (aux != NULL) {
        if (aux->info.id == idCautat)
            return aux->info;
        aux = aux->next;
    }

    Masina m;
    m.id = -1;
    return m;
}

int main() {

    HashTable ht = citireMasiniDinFisier("masini2.txt");

    printf("=== TABELA ===\n");
    afisareTabelaDeMasini(ht);

    printf("=== CAUTARE ===\n");
    Masina m = getMasinaDupaCheie(ht, 3);
    

    dezalocareTabelaDeMasini(&ht);

    return 0;
}
