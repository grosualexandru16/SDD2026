#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Masina citireMasinaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);

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

	aux = strtok(NULL, sep);
	m.serie = *aux;

	return m;









}

void afisareMasina(Masina m) {

	printf("ID: %d", m.id);
	printf("Numar Usi: %d", m.nrUsi);
	printf("Pret: %5.2f", m.pret);
	printf("Model: %s", m.model);
	printf("Nume Sofer: %s", m.numeSofer);
	printf("Serie: %c", m.serie);
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
		while (p->next) {

			p = p->next;

		}
		p->next = nou;
	}
}



void adaugaMasiniInLista2(Nod** lista, Masina m) {

	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;

	if (lista == NULL) {
	
		*lista = nou;
	
	}

	else {
		
		Nod* p = *lista;

		while (p->next) {
			p = p->next;

		}
	
		p->next = nou;
	
	}
	

	}


	void afisareListaMasini(Nod * lista) {

		while (lista) {
			afisareMasina(lista->info);
			lista = lista->next;
		}
	}


	void afisareListaMasini2(Nod* lista) {

		while (lista) {

			afisareMasina(lista->info);
			lista = lista->next;
		}


	}

	Nod* citireMasiniDinFisier(const char* numeFisier){

		FILE* f = fopen(numeFisier, "r");
		Nod* lista = NULL;

		while (!feof(f)) {

			Masina m = citireMasinaDinFisier(f);
			adaugaMasinaInLista(&lista, m);
		}
		fclose(f);

		return lista;

	}



	Nod* citireMasiniDinFisier2(const char* numeFisier) {

		FILE* file = fopen(numeFisier, "r");
		Nod* lista = NULL;

		while (!feof(file)) {

			Masina m = citireMasinaDinFisier(file);
			adaugaMasinaInLista(&lista, m);

		}

		fclose(file);

		return lista;




	}



	void dezalocareListaMasini(Nod** lista) {
		while (*lista) {
			free((*lista)->info.model);
			free((*lista)->info.numeSofer);
			Nod* p = *lista;
			(*lista) = (*lista)->next;
			free(p);
		}

		free(lista);


	}

	void dezalocareListaMasini2(Nod** lista) {

		while (*lista) {
			free((*lista)->info.numeSofer);
			free((*lista)->info.model);
			Nod* p = *lista;
			(*lista)->next = p;
			free(p);


		}


	}


	int main() {


		Nod* lista = citireMasiniDinFisier("masini2.txt");
		afisareListaMasini(lista);




		return 0;

	
	
	}




 