#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct Nod Nod;
typedef struct Lista Lista;

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
	Nod* prev;
	Nod* next;


};

struct Lista {
	Nod* prim;
	Nod* ultim;


};


Masina citireMasinaDinFisier(FILE* f) {

	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
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

void adaugaMasinaInLista(Lista* lista, Masina m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;
	nou->next = nou->prev = NULL;

	if (lista->ultim) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {

		lista->prim = lista->ultim = nou;
	}
}

void adaugaMasinaInLista(Lista* lista, Masina m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;
	nou->prev = NULL;

	if (lista->ultim) {

		
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;

	}

	else {

		lista->prim = lista->ultim = nou;
	}



}

	void adaugaMasinaLaInceputDeLista(Lista* lista, Masina m) {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = m;
		nou->next = nou->prev = NULL;

		if (lista->prim) {
			nou->next = lista->prim;
			lista->prim->prev = nou;
			lista->prim = nou;
		}
		else {
			lista->prim = lista->ultim = nou;
		}

	}

	Lista* adaugaMasiniDinFisier(const char* numeFisier) {

		FILE* file = fopen(numeFisier, "r");
		if (file) {
			Lista* lista = malloc(sizeof(Lista));
			lista->prim = NULL;
			lista->ultim = NULL;

			while (!feof(file)) {

				adaugaMasinaInLista(lista, citireMasinaDinFisier(file));
			}
			fclose(file);
			return lista;

		}

		else {
			Lista* lista = malloc(sizeof(Lista));
			lista->prim = NULL;
			lista->ultim = NULL;
			return lista;


		}

	}

	void afisareMasina(Masina m) {
		printf("Id: %d", m.id);
		printf("Numar usi %d", m.nrUsi);
		printf("Pret: %5.2f", m.pret);
		printf("Model: %s", m.model);
		printf("Nume Sofer: %s", m.numeSofer);
		printf("Serie: %c", m.serie);

	}

	void afisareMasinaDinLista(Lista* listaDubla) {
		Nod* p = listaDubla->prim;
		while (p) {
			afisareMasina(p->info);
			p = p->next;

		}

	}

	float calculeazaPretMediu(Lista* listaDubla) {
		Nod* p = listaDubla->prim;

		float suma= 0;
		int count = 0;

		while (p) {
			suma = suma + p->info.pret;
			count++;
			p = p->next;

		}

		if (count > 0) {

			float average = suma / count;
			return average;
		}

		else {
			return 0;
		}




	}

	void dezalocareLista(Lista** lista) {

		Nod* p = (*lista)->prim;

		while (p) {
			free(p->info.model);
			free(p->info.numeSofer);

			Nod* aux = p;
			p = p->next;
			free(aux);
		

		}

		free(*lista);
		lista = NULL;


	}




int main() {

	printf("Programul porneste");


	Lista* listaDubla = adaugaMasiniDinFisier("masini2.txt");

	afisareMasinaDinLista(listaDubla);

	printf("Pret Mediu: %5.2f", calculeazaPretMediu(listaDubla));



	return 0;
}