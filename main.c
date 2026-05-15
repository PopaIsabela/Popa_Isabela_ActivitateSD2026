#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char* nume;
	float salariu;
	double bonus;
} Angajat;

typedef struct Nod {
	Angajat info;
	struct Nod* next;
} Nod;

Angajat copiereAngajat(Angajat a) {
	Angajat copie;
	copie.id = a.id;
	copie.salariu = a.salariu;
	copie.bonus = a.bonus;
	copie.nume = (char*)malloc((strlen(a.nume) + 1) * sizeof(char));
	strcpy(copie.nume, a.nume);
	return copie;
}

Nod* creareNod(Angajat a) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereAngajat(a);
	nou->next = NULL;
	return nou;
}

Nod* inserareInceput(Nod* cap, Angajat a) {
	Nod* nou = creareNod(a);
	nou->next = cap;
	return nou;
}

Nod* inserareFinal(Nod* cap, Angajat a) {
	Nod* nou = creareNod(a);

	if (cap == NULL) {
		return nou;
	}

	Nod* temp = cap;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	temp->next = nou;
	return cap;
}

void afisareAngajat(Angajat a) {
	printf("Id: %d, Nume: %s, Salariu: %.2f, Bonus: %.2lf\n",
		a.id, a.nume, a.salariu, a.bonus);
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisareAngajat(cap->info);
		cap = cap->next;
	}
}

Nod* stergereDupaId(Nod* cap, int id) {
	Nod* temp = cap;
	Nod* prev = NULL;

	while (temp != NULL && temp->info.id != id) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		return cap;
	}

	if (prev == NULL) {
		cap = temp->next;
	}
	else {
		prev->next = temp->next;
	}

	free(temp->info.nume);
	free(temp);

	return cap;
}

int numarNoduri(Nod* cap) {
	int nr = 0;

	while (cap != NULL) {
		nr++;
		cap = cap->next;
	}

	return nr;
}

Angajat* conversieVector(Nod* cap, int* n) {
	*n = numarNoduri(cap);

	if (*n == 0) {
		return NULL;
	}

	Angajat* vect = (Angajat*)malloc((*n) * sizeof(Angajat));

	int i = 0;
	while (cap != NULL) {
		vect[i] = copiereAngajat(cap->info);
		i++;
		cap = cap->next;
	}

	return vect;
}

Angajat* extrageFiltru(Nod* cap, int* n, float prag) {
	*n = 0;

	Nod* temp = cap;
	while (temp != NULL) {
		if (temp->info.salariu > prag) {
			(*n)++;
		}
		temp = temp->next;
	}

	if (*n == 0) {
		return NULL;
	}

	Angajat* vect = (Angajat*)malloc((*n) * sizeof(Angajat));

	int i = 0;
	while (cap != NULL) {
		if (cap->info.salariu > prag) {
			vect[i] = copiereAngajat(cap->info);
			i++;
		}
		cap = cap->next;
	}

	return vect;
}

float salariuMediu(Nod* cap) {
	if (cap == NULL) {
		return 0;
	}

	float suma = 0;
	int nr = 0;

	while (cap != NULL) {
		suma += cap->info.salariu;
		nr++;
		cap = cap->next;
	}

	return suma / nr;
}

Nod* cautareDupaId(Nod* cap, int id) {
	while (cap != NULL) {
		if (cap->info.id == id) {
			return cap;
		}
		cap = cap->next;
	}

	return NULL;
}

Angajat angajatBonusMaxim(Nod* cap) {
	Angajat maxim = cap->info;

	while (cap != NULL) {
		if (cap->info.bonus > maxim.bonus) {
			maxim = cap->info;
		}
		cap = cap->next;
	}

	return maxim;
}

void sortareVectorDupaSalariu(Angajat* vect, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (vect[i].salariu > vect[j].salariu) {
				Angajat aux = vect[i];
				vect[i] = vect[j];
				vect[j] = aux;
			}
		}
	}
}

void dezalocareVector(Angajat* vect, int n) {
	if (vect != NULL) {
		for (int i = 0; i < n; i++) {
			free(vect[i].nume);
		}
		free(vect);
	}
}

void dezalocareLista(Nod* cap) {
	while (cap != NULL) {
		Nod* aux = cap;
		cap = cap->next;

		free(aux->info.nume);
		free(aux);
	}
}

Nod* citireDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	if (f == NULL) {
		printf("Eroare la deschiderea fisierului %s!\n", numeFisier);
		return NULL;
	}

	Nod* cap = NULL;
	Angajat a;
	char buffer[100];

	while (fscanf(f, "%d %s %f %lf", &a.id, buffer, &a.salariu, &a.bonus) == 4) {
		a.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(a.nume, buffer);

		cap = inserareFinal(cap, a);

		free(a.nume);
	}

	fclose(f);
	return cap;
}

void salvareInFisier(Nod* cap, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "w");

	if (f == NULL) {
		printf("Eroare la deschiderea fisierului pentru scriere!\n");
		return;
	}

	while (cap != NULL) {
		fprintf(f, "%d %s %.2f %.2lf\n",
			cap->info.id,
			cap->info.nume,
			cap->info.salariu,
			cap->info.bonus);

		cap = cap->next;
	}

	fclose(f);
}

int main() {
	Nod* cap = NULL;

	Angajat a1 = { 1, "Beatrice", 9600.0f, 700.87 };
	Angajat a2 = { 2, "Cosmin", 16000.9f, 10000.00 };
	Angajat a3 = { 3, "Andreea", 8200.5f, 1200.50 };

	cap = inserareInceput(cap, a1);
	cap = inserareFinal(cap, a2);
	cap = inserareFinal(cap, a3);

	printf("Lista angajati:\n");
	afisareLista(cap);

	printf("\nNumar angajati: %d\n", numarNoduri(cap));
	printf("Salariu mediu: %.2f\n", salariuMediu(cap));

	Angajat bonusMaxim = angajatBonusMaxim(cap);
	printf("\nAngajatul cu bonusul maxim:\n");
	afisareAngajat(bonusMaxim);

	int idCautat = 2;
	Nod* gasit = cautareDupaId(cap, idCautat);

	if (gasit != NULL) {
		printf("\nAngajat gasit dupa id:\n");
		afisareAngajat(gasit->info);
	}
	else {
		printf("\nNu exista angajat cu id-ul %d.\n", idCautat);
	}

	cap = stergereDupaId(cap, 2);
	printf("\nLista dupa stergere:\n");
	afisareLista(cap);

	int n = 0;
	Angajat* vect = conversieVector(cap, &n);

	printf("\nVectorul de angajati:\n");
	for (int i = 0; i < n; i++) {
		printf("V[%d] = %s\n", i, vect[i].nume);
	}

	sortareVectorDupaSalariu(vect, n);

	printf("\nVector sortat dupa salariu:\n");
	for (int i = 0; i < n; i++) {
		afisareAngajat(vect[i]);
	}

	dezalocareVector(vect, n);

	int nrFiltrati = 0;
	Angajat* filtrati = extrageFiltru(cap, &nrFiltrati, 9000);

	printf("\nAngajati cu salariu peste 9000:\n");
	for (int i = 0; i < nrFiltrati; i++) {
		afisareAngajat(filtrati[i]);
	}

	dezalocareVector(filtrati, nrFiltrati);

	dezalocareLista(cap);

	cap = citireDinFisier("angajati.txt");

	printf("\nLista citita din fisier:\n");
	afisareLista(cap);

	if (cap != NULL) {
		Angajat bonusMaximFisier = angajatBonusMaxim(cap);
		printf("\nAngajatul din fisier cu bonusul maxim:\n");
		afisareAngajat(bonusMaximFisier);
	}

	salvareInFisier(cap, "angajatiOut.txt");

	Nod* listaNoua = citireDinFisier("angajatiOut.txt");

	printf("\nLista citita din angajatiOut.txt:\n");
	afisareLista(listaNoua);

	dezalocareLista(cap);
	dezalocareLista(listaNoua);

	return 0;
}