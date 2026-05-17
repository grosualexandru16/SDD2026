#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Masina {

    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;

};

typedef struct Masina Masina;

struct Heap {

    int lungime;
    Masina* masini;
    int nrElemente;

};

typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* f) {

    char buffer[100];
    char sep[3] = ",\n";

    if (!fgets(buffer, 100, f))
    {
        Masina m;
        m.id = 0; 
        return m;
    }

    Masina m;
    char* aux;

    aux = strtok(buffer, sep);
    m.id = atoi(aux);

    aux = strtok(NULL, sep);
    m.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    m.pret = atof(aux);

    aux = strtok(NULL, sep);
    m.model = malloc(strlen(aux) + 1);
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m.numeSofer, aux);

    m.serie = *(strtok(NULL, sep));

    return m;
}

void afisareMasina(Masina m) {

    printf("Id: %d\n", m.id);
    printf("Numar Usi: %d\n", m.nrUsi);
    printf("Pret: %5.2f\n", m.pret);
    printf("Model: %s\n", m.model);
    printf("Nume Sofer: %s\n", m.numeSofer);
    printf("Serie: %c\n\n", m.serie);

}

Heap initializareHeap(int lungime) {

    Heap h;

    h.lungime = lungime;
    h.nrElemente = 0;
    h.masini = malloc(sizeof(Masina) * lungime);

    return h;

}

void filtreazaHeap(Heap heap, int pozNod) {

    int pozSt = 2 * pozNod + 1;
    int pozDr = 2 * pozNod + 2;
    int pozMin = pozNod;

    if (pozSt < heap.nrElemente && heap.masini[pozMin].id > heap.masini[pozSt].id) {
        pozMin = pozSt;
    }

    if (pozDr < heap.nrElemente && heap.masini[pozMin].id > heap.masini[pozDr].id) {
        pozMin = pozDr;
    }

    if (pozNod != pozMin) {

        Masina aux = heap.masini[pozMin];
        heap.masini[pozMin] = heap.masini[pozNod];
        heap.masini[pozNod] = aux;

        if (pozMin <= ((heap.nrElemente - 2) / 2)) {
            filtreazaHeap(heap, pozMin);
        }
    }
}

Heap creareHeapDinFisier(const char* numeFisier) {

    Heap heap = initializareHeap(10);
    heap.nrElemente = 0;

    FILE* f = fopen(numeFisier, "r");

    while (!feof(f)) {

        Masina m = citireMasinaDinFisier(f);

         
            heap.masini[heap.nrElemente++] = m;
       
    }

    fclose(f);

    for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
        filtreazaHeap(heap, i);
    }

    return heap;
}

void afisareHeap(Heap heap) {

    for (int i = 0; i < heap.nrElemente; i++) {
        afisareMasina(heap.masini[i]);
    }
}

void afisareHeapAscuns(Heap heap) {

    for (int i = heap.nrElemente; i < heap.lungime; i++) {
        afisareMasina(heap.masini[i]);
    }
}

Masina extragereMasina(Heap* heap) {

    Masina m;

    if (heap->nrElemente > 0) {

        m = heap->masini[0];

        heap->masini[0] = heap->masini[heap->nrElemente - 1];
        heap->masini[heap->nrElemente - 1] = m;

        heap->nrElemente--;

        for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
            filtreazaHeap(*heap, i);
        }
    }

    return m;
}

void dezalocareHeap(Heap* h) {

    for (int i = 0; i < h->lungime; i++) {
        free(h->masini[i].model);
        free(h->masini[i].numeSofer);
    }

    free(h->masini);
    h->masini = NULL;
    h->lungime = 0;
    h->nrElemente = 0;
}

int main() {

    Heap h = creareHeapDinFisier("masini2.txt");

    afisareHeap(h);

    Masina m1 = extragereMasina(&h);
    afisareHeap(h);
    afisareMasina(m1);

    afisareMasina(extragereMasina(&h));

    dezalocareHeap(&h);

    return 0;
}