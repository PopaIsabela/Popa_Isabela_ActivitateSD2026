#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 10

typedef struct Student {
	int id;
	char* nume;
	float medie;
} Student;

typedef struct Nod {
	Student info;
	struct Nod* next;
} Nod;

typedef struct HashTable {
	Nod** vector;
	int dimensiune;
} HashTable;

Student copiereStudent(Student s) {
	Student copie;
	copie.id = s.id;
	copie.medie = s.medie;
	copie.nume = (char*)malloc(strlen(s.nume) + 1);
	strcpy(copie.nume, s.nume);
	return copie;
}

int functieHash(int id, int dimensiune) {
	return id % dimensiune;
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dimensiune = dimensiune;
	ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));

	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

void inserareHashTable(HashTable ht, Student s) {
	int pozitie = functieHash(s.id, ht.dimensiune);

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereStudent(s);
	nou->next = ht.vector[pozitie];

	ht.vector[pozitie] = nou;
}

void afisareStudent(Student s) {
	printf("Id: %d, Nume: %s, Medie: %.2f\n", s.id, s.nume, s.medie);
}

void afisareHashTable(HashTable ht) {
	for (int i = 0; i < ht.dimensiune; i++) {
		printf("\nPozitia %d:\n", i);

		if (ht.vector[i] == NULL) {
			printf("Lista vida\n");
		}
		else {
			Nod* temp = ht.vector[i];

			while (temp != NULL) {
				afisareStudent(temp->info);
				temp = temp->next;
			}
		}
	}
}

Student* cautareStudent(HashTable ht, int id) {
	int pozitie = functieHash(id, ht.dimensiune);
	Nod* temp = ht.vector[pozitie];

	while (temp != NULL) {
		if (temp->info.id == id) {
			return &(temp->info);
		}

		temp = temp->next;
	}

	return NULL;
}

int stergereStudent(HashTable ht, int id) {
	int pozitie = functieHash(id, ht.dimensiune);

	Nod* temp = ht.vector[pozitie];
	Nod* anterior = NULL;

	while (temp != NULL && temp->info.id != id) {
		anterior = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		return 0;
	}

	if (anterior == NULL) {
		ht.vector[pozitie] = temp->next;
	}
	else {
		anterior->next = temp->next;
	}

	free(temp->info.nume);
	free(temp);

	return 1;
}

int numarStudenti(HashTable ht) {
	int nr = 0;

	for (int i = 0; i < ht.dimensiune; i++) {
		Nod* temp = ht.vector[i];

		while (temp != NULL) {
			nr++;
			temp = temp->next;
		}
	}

	return nr;
}

float mediaGenerala(HashTable ht) {
	float suma = 0;
	int nr = 0;

	for (int i = 0; i < ht.dimensiune; i++) {
		Nod* temp = ht.vector[i];

		while (temp != NULL) {
			suma += temp->info.medie;
			nr++;
			temp = temp->next;
		}
	}

	if (nr == 0) {
		return 0;
	}

	return suma / nr;
}

int numarElementeBucket(HashTable ht, int pozitie) {
	int nr = 0;

	if (pozitie < 0 || pozitie >= ht.dimensiune) {
		return 0;
	}

	Nod* temp = ht.vector[pozitie];

	while (temp != NULL) {
		nr++;
		temp = temp->next;
	}

	return nr;
}

void afisareBucketMaxim(HashTable ht) {
	int pozitieMaxima = 0;
	int nrMaxim = 0;

	for (int i = 0; i < ht.dimensiune; i++) {
		int nr = numarElementeBucket(ht, i);

		if (nr > nrMaxim) {
			nrMaxim = nr;
			pozitieMaxima = i;
		}
	}

	printf("\nBucket-ul cu cele mai multe elemente este pozitia %d, cu %d elemente:\n",
		pozitieMaxima, nrMaxim);

	Nod* temp = ht.vector[pozitieMaxima];

	while (temp != NULL) {
		afisareStudent(temp->info);
		temp = temp->next;
	}
}

void afisareFactorIncarcare(HashTable ht) {
	int nr = numarStudenti(ht);
	float factor = (float)nr / ht.dimensiune;

	printf("\nFactor de incarcare hash table: %.2f\n", factor);
}

HashTable redimensionareHashTable(HashTable ht) {
	HashTable nou = initializareHashTable(ht.dimensiune * 2);

	for (int i = 0; i < ht.dimensiune; i++) {
		Nod* temp = ht.vector[i];

		while (temp != NULL) {
			inserareHashTable(nou, temp->info);
			temp = temp->next;
		}
	}

	return nou;
}

void dezalocareHashTable(HashTable* ht) {
	for (int i = 0; i < ht->dimensiune; i++) {
		Nod* temp = ht->vector[i];

		while (temp != NULL) {
			Nod* aux = temp;
			temp = temp->next;

			free(aux->info.nume);
			free(aux);
		}
	}

	free(ht->vector);
	ht->vector = NULL;
	ht->dimensiune = 0;
}

int main() {
	HashTable ht = initializareHashTable(DIM);

	Student s1 = { 101, "Maria", 9.45f };
	Student s2 = { 112, "Vlad", 8.75f };
	Student s3 = { 123, "Elena", 9.90f };
	Student s4 = { 134, "Rares", 7.80f };
	Student s5 = { 145, "Ioana", 8.60f };
	Student s6 = { 111, "Daria", 9.10f };
	Student s7 = { 121, "Alex", 7.95f };

	inserareHashTable(ht, s1);
	inserareHashTable(ht, s2);
	inserareHashTable(ht, s3);
	inserareHashTable(ht, s4);
	inserareHashTable(ht, s5);
	inserareHashTable(ht, s6);
	inserareHashTable(ht, s7);

	printf("Tabela hash initiala:\n");
	afisareHashTable(ht);

	printf("\nNumar studenti: %d\n", numarStudenti(ht));
	printf("Media generala: %.2f\n", mediaGenerala(ht));

	afisareFactorIncarcare(ht);
	afisareBucketMaxim(ht);

	int idCautat = 123;
	Student* studentGasit = cautareStudent(ht, idCautat);

	if (studentGasit != NULL) {
		printf("\nStudent gasit:\n");
		afisareStudent(*studentGasit);
	}
	else {
		printf("\nNu exista student cu id-ul %d.\n", idCautat);
	}

	int idSters = 112;

	if (stergereStudent(ht, idSters)) {
		printf("\nStudentul cu id-ul %d a fost sters.\n", idSters);
	}
	else {
		printf("\nStudentul cu id-ul %d nu exista.\n", idSters);
	}

	printf("\nTabela hash dupa stergere:\n");
	afisareHashTable(ht);

	printf("\n\nHash table dupa redimensionare:\n");
	HashTable htNou = redimensionareHashTable(ht);
	afisareHashTable(htNou);

	printf("\nStatistici dupa redimensionare:\n");
	afisareFactorIncarcare(htNou);
	afisareBucketMaxim(htNou);

	dezalocareHashTable(&htNou);
	dezalocareHashTable(&ht);

	return 0;
}