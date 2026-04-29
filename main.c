#define  _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
	char* nume;
	float salariu;
	double bonus;
}Angajat;

typedef struct Nod {
	Angajat info;
	struct Nod* next;
}Nod;

//inserare inceput
Nod* inserareInceput(Nod* cap, Angajat a) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info.id = a.id;
	nou->info.salariu = a.salariu;
	nou->info.bonus = a.bonus;
	nou->info.nume = (char*)malloc(( strlen(a.nume) + 1)* sizeof(char));
	strcpy(nou->info.nume, a.nume);
	nou->next = cap;
	return nou;
}

//inserare final
Nod* inserareFinal(Nod* cap, Angajat a) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info.id = a.id;
	nou->info.salariu = a.salariu;
	nou->info.bonus = a.bonus;
	nou->info.nume = (char*)malloc((strlen(a.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, a.nume);
	nou->next = NULL;
	if (!cap) 
		return nou;
	Nod* temp = cap;
	while (temp->next)
		temp = temp->next;
	temp->next = nou;
	return cap;
}

//traversare
void afisareLista(Nod* cap) {
	while (cap) {
		printf("Id: %d, Nume: %s, Salariu: %.2f, Bonus: %.2lf\n",
			cap->info.id, cap->info.nume, cap->info.salariu, cap->info.bonus);
		cap = cap->next;
	}
}
	//stregere dupa id
	Nod* stergereDupaId(Nod * cap, int id) {
		Nod* temp = cap;
		Nod* prev = NULL;
		while (temp && temp->info.id != id) {
			prev = temp;
			temp = temp->next;
		}
		if (!temp)
			return cap;
		if (prev)
			prev->next = temp->next;
		else
			cap = temp->next;
		free(temp->info.nume);
		free(temp);
		return cap;
	}

	//conversie in vector
	void conversieVector(Nod* cap, Angajat* vect, int* n) {
		while (cap) {
			vect[*n] = cap->info;
			(*n)++;
			cap = cap->next;
		}
	}

	// extrage dupa filtru
	void extrageFiltru(Nod* cap, Angajat* vect, int* n, float prag) {
		while (cap) {
			if (cap->info.salariu > prag) {
				vect[*n] = cap->info;
				(*n)++;
			}
			cap = cap->next;
		}
	}


//dezalocare
	void dezalocare(Nod* cap) {
		while (cap) {
			Nod* aux = cap;
			cap = cap->next;
			free(aux->info.nume);
			free(aux);
		}
	}

	//citire din fisier
	Nod* citireDinFisier(const char* numeFisier) {
		FILE* f = fopen(numeFisier, "r");
		if (!f) {
			printf("Eroare la deschiderea fisierului! \n");
			return NULL;
		}

		Nod* cap = NULL;
		Angajat a;
		char buffer[100];
		while (fscanf(f, "%d %s %f %lf", &a.id, buffer, &a.salariu, &a.bonus) == 4) {
			a.nume = (char*)malloc(strlen(buffer) + 1);
			strcpy(a.nume, buffer);
			cap = inserareFinal(cap, a);
			free(a.nume);
		}
		fclose(f);
		return cap;
	}

	//salvare in fisier
	void salvareInFisier(Nod* cap, const char* numeFisier) {
		FILE* f = fopen(numeFisier, "w");
		if (!f) {
			printf("Eroare la deschiderea fisierului pentru scriere! \n");
			return;
		}

		while (cap) {
			fprintf(f, "%d %s %.2f %.2lf\n",
				cap->info.id, cap->info.nume, cap->info.salariu, cap->info.bonus);
			cap = cap->next;
		}
		fclose(f);
	}

	//testare
	int main() {

		Nod* cap = NULL;
		Angajat a1 = { 1, "Beatrice", 9600.0, 700.87 };
		Angajat a2 = { 2, "Cosmin", 16000.9, 10000.00 };

		cap = inserareInceput(cap, a1);
		cap = inserareFinal(cap, a2);

		printf("Lista angajati: \n");
		afisareLista(cap);

		cap = stergereDupaId(cap, 2);
		printf("\n Lista dupa stergere: \n");
		afisareLista(cap);

		Angajat vect[100];
		int n = 0;
		conversieVector(cap, vect, &n);
		printf("\n Vectorul de angajati: \n'");
		for (int i = 0; i < n; i++)
			printf("V[%d] =%s\n", i, vect[i].nume);

		//test citire din fisier
		cap = citireDinFisier("angajati.txt");
		printf("\n Lista dupa citirea din fisier:\n");
		afisareLista(cap);
	
	

		//test salvare in fisier
		salvareInFisier(cap, "angajatiOut.txt");
		Nod* listaNoua = citireDinFisier("angajatiOut.txt");
		printf("\n Lista dupa salvarea in fisier: \n");
		afisareLista(listaNoua);

		dezalocare(cap);
		dezalocare(listaNoua);


		return 0;
	}