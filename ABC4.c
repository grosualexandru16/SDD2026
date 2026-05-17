#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};

struct Nod {
    Masina info;
    Nod* st;
    Nod* dr;
};

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

void adaugareMasinaInArbore(Nod** rad, Masina m) {
    if (*rad == NULL) {
        Nod* nou = malloc(sizeof(Nod));
        nou->info = m;
        nou->st = NULL;
        nou->dr = NULL;
        *rad = nou;
    }
    else {
        if (m.id < (*rad)->info.id)
            adaugareMasinaInArbore(&((*rad)->st), m);
        else if (m.id > (*rad)->info.id)
            adaugareMasinaInArbore(&((*rad)->dr), m);
    }
}

Nod* citireArboreMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* rad = NULL;

    if (f) {
        while (!feof(f)) {
            Masina m = citireMasinaDinFisier(f);
            if (m.id != 0)
                adaugareMasinaInArbore(&rad, m);
        }
        fclose(f);
    }

    return rad;
}



void afisareInordine(Nod* rad) {
    if (rad) {
        afisareInordine(rad->st);
        afisareMasina(rad->info);
        afisareInordine(rad->dr);
    }
}

void afisarePreordine(Nod* rad) {
    if (rad) {
        afisareMasina(rad->info);
        afisarePreordine(rad->st);
        afisarePreordine(rad->dr);
    }
}

void afisarePostordine(Nod* rad) {
    if (rad) {
        afisarePostordine(rad->st);
        afisarePostordine(rad->dr);
        afisareMasina(rad->info);
    }
}

void dezalocareArbore(Nod** rad) {
    if (*rad) {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);

        free((*rad)->info.model);
        free((*rad)->info.numeSofer);

        free(*rad);
        *rad = NULL;
    }
}



Masina getMasinaByID(Nod* rad, int id) {
    if (rad == NULL) {
        Masina m;
        m.id = 0;
        return m;
    }

    if (id == rad->info.id)
        return rad->info;

    if (id < rad->info.id)
        return getMasinaByID(rad->st, id);
    else
        return getMasinaByID(rad->dr, id);
}

int determinaNumarNoduri(Nod* rad) {
    if (rad == NULL)
        return 0;
    return 1 + determinaNumarNoduri(rad->st) + determinaNumarNoduri(rad->dr);
}

int calculeazaInaltimeArbore(Nod* rad) {
    if (rad == NULL)
        return 0;

    int hSt = calculeazaInaltimeArbore(rad->st);
    int hDr = calculeazaInaltimeArbore(rad->dr);

    return 1 + (hSt > hDr ? hSt : hDr);
}

float calculeazaPretTotal(Nod* rad) {
    if (rad == NULL)
        return 0;
    return rad->info.pret +
        calculeazaPretTotal(rad->st) +
        calculeazaPretTotal(rad->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
    if (rad == NULL)
        return 0;

    float suma = 0;
    if (strcmp(rad->info.numeSofer, numeSofer) == 0)
        suma += rad->info.pret;

    suma += calculeazaPretulMasinilorUnuiSofer(rad->st, numeSofer);
    suma += calculeazaPretulMasinilorUnuiSofer(rad->dr, numeSofer);

    return suma;
}

int main() {

    Nod* rad = citireArboreMasiniDinFisier("masini2.txt");

   
    afisareInordine(rad);

  
    afisarePreordine(rad);

    
    afisarePostordine(rad);

  
    printf("%d\n", determinaNumarNoduri(rad));

  
    printf("%d\n", calculeazaInaltimeArbore(rad));


    printf("%.2f\n", calculeazaPretTotal(rad));

   
    printf("%.2f\n", calculeazaPretulMasinilorUnuiSofer(rad, "Ion"));

   
    Masina m = getMasinaByID(rad, 5);
    if (m.id != 0)
        afisareMasina(m);

    dezalocareArbore(&rad);

    return 0;
}
