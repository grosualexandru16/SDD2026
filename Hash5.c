#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;
typedef struct Hash Hash;

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
	Nod* next;
};


struct Hash {
	
	int dim;
	Nod** vector;

};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

afisareListaMasini(Nod* lista) {

	if (lista) {
		while (lista) {
			afisareMasina(lista->info);
			lista = lista->next;
		 }

	}

}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua){

	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if (*lista == NULL) {

		*lista = nou;

	}

	else {

		Nod* aux = *lista;

		while(aux->next != NULL) {

			aux = aux->next;
		}

		aux->next = nou;

	}


}

Hash initializareHashTable(int dim) {

	Hash ht;

	ht.dim = dim;

	ht.vector = malloc(sizeof(Nod*) * dim);

	for (int i = 0; i < dim; i++) {

		ht.vector[i] = NULL;

	}

	return ht;
}

int calculeazaHash(int id, int dimensiune) {

	return id % dimensiune;
}

void inserareMasinaInTabela(Hash tabela, Masina masinaNoua) {

	if (tabela.dim > 0) {
		int id = masinaNoua.id;

		int pozitie = calculeazaHash(id, tabela.dim);

		if (pozitie >= 0 && pozitie < tabela.dim) {

			adaugaMasinaInLista(&(tabela.vector[pozitie]), masinaNoua);
		}
	}

}


Hash inserareMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");

	Hash ht = initializareHashTable(10);

	while (!feof(f)) {

		inserareMasinaInTabela(ht, citireMasinaDinFisier(f));

	}


		fclose(f);
	return ht;
}

void afisareTabela(Hash table) {

	if (table.dim > 0) {


		for (int i = 0; i < table.dim; i++) {

			afisareListaMasini(table.vector[i]);


		}

	}
}

void dezalocareListaDeMasini(Nod** cap) {

	while (*cap) {

		free((*cap)->info.model);
		free((*cap)->info.numeSofer);

		Nod* copy = *cap;
		*cap = (*cap)->next;
		free(copy);
	}

}

void dezalocareHash(Hash* ht) {

	for (int i = 0; i < ht->dim; i++) {

		dezalocareListaDeMasini(&(ht->vector[i]));

	}

	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;


}



int main() {

	printf("Start Program");

	Hash ht = inserareMasiniDinFisier("masini2.txt");

	afisareTabela(ht);

	dezalocareHash(&ht);



	return 0;
}