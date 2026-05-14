#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

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
	Nod* St;
	Nod* Dr;

};

Masina citireMasinaDinFisier(FILE* f){

	char buffer[100];
	char sep[3] = ",\n";

	Masina m;

	fgets(buffer, 100, f);

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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** rad, Masina masinaNoua) {

	if (*rad == NULL) {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->St = NULL;
		nou->Dr = NULL;

		*rad = nou;
	}
	
	else {

		if ((*rad)->info.id > masinaNoua.id) {

			adaugaMasinaInArbore(&((*rad)->St), masinaNoua);
		}

		if ((*rad)->info.id < masinaNoua.id) {

			adaugaMasinaInArbore(&((*rad)->Dr), masinaNoua);
		}

	}
}


Nod* citireArboreMasiniDinFisier(const char* numeFisier) {

	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {

		while (!feof(f)) {
			Masina masinaNoua = citireMasinaDinFisier(f);
			adaugaMasinaInArbore(&rad, masinaNoua);
		}
	}

	fclose(f);

	return rad;
}

void afisareMasiniDinArbore(Nod* rad) {

	if (rad) {
		
		afisareMasiniDinArbore(rad->St);
		afisareMasina(rad->info);
		afisareMasiniDinArbore(rad->Dr);

	}

}

void afisarePreordine(Nod* rad) {

	afisareMasina(rad->info);
	afisarePreordine(rad->Dr);
	afisarePreordine(rad->St);

}

void dezalocareArbore(Nod** rad) {

	if (*rad) {

		dezalocareArbore(&(*rad)->St);
		dezalocareArbore(&(*rad)->Dr);
		free((*rad)->info.numeSofer);
		free((*rad)->info.model);
		free(*rad);
		*rad = NULL;

	}

}

Masina getMasinaById(Nod* rad, int idCautat) {

	Masina m;
	m.id = -1;

	if (rad->info.id == idCautat) {

		m = rad->info;
		m.model = malloc(strlen(rad->info.model) + 1);
		strcpy(m.model, rad->info.model);

		m.numeSofer = malloc(strlen(rad->info.numeSofer) + 1);
		strcpy(m.numeSofer, rad->info.numeSofer);


	}

	if (rad->info.id > idCautat) {

		m = getMasinaById(rad->St, idCautat);

	}

	if (rad->info.id < idCautat) {

		m = getMasinaById(rad->Dr, idCautat);

	}

	return m;


}

int calculeazaInaltimeArbore(Nod* rad) {

	return calculeazaInaltimeArbore(rad->St) + calculeazaInaltimeArbore(rad->Dr) + 1;

}


int main() {

	printf("Start Program\n");

	//Nod* rad = NULL;

	Nod* rad = citireArboreMasiniDinFisier("masini2.txt");

	afisareMasiniDinArbore(rad);

	Masina m = getMasinaById(rad, 10);
	afisareMasina(m);

	dezalocareArbore(&rad);


	return 0;
}