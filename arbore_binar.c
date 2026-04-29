// arbori binari de cautare - Librarii
// SRD, RSD, SDR

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Librarie Librarie;
typedef struct Nod Nod;

struct Librarie
{
	int id;
	int nrCarti;
	char** carti;
};

struct Nod
{
	Librarie info;
	Nod* st;
	Nod* dr;
};

void inserareInArbore(Nod** radacina, Librarie l)
{
	if (*radacina)
	{
		if ((*radacina)->info.id > l.id)
		{
			inserareInArbore(&((*radacina)->st), l);
		}
		else
		{
			inserareInArbore(&((*radacina)->dr), l);
		}
	}
	else
	{
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->st = NULL;
		nod->dr = NULL;
		nod->info = l;
		*radacina = nod;
	}
}

void afisareLibrarie(Librarie librarie)
{
	printf("Libraria cu id-ul %d are %d carti: ",
		librarie.id, librarie.nrCarti);

	for (int i = 0; i < librarie.nrCarti; i++)
	{
		printf("%s, ", librarie.carti[i]);
	}
	printf("\n");
}

void afisareArborePreordine(Nod* rad) // RSD
{
	if (rad)
	{
		afisareLibrarie(rad->info);
		afisareArborePreordine(rad->st);
		afisareArborePreordine(rad->dr);
	}
}

void afisareArboreInordine(Nod* rad) // SRD
{
	if (rad)
	{
		afisareArboreInordine(rad->st);
		afisareLibrarie(rad->info);
		afisareArboreInordine(rad->dr);
	}
}

int calculNrCarti(Nod* rad)
{
	if (rad)
	{
		int suma = rad->info.nrCarti;
		suma += calculNrCarti(rad->st);
		suma += calculNrCarti(rad->dr);
		return suma;
	}
	return 0;
}

Librarie citireLibrarie(FILE* f)
{
	Librarie librarie;

	fscanf(f, "%d", &librarie.id);
	fscanf(f, "%d", &librarie.nrCarti);

	if (librarie.nrCarti > 0)
	{
		librarie.carti = (char**)malloc(sizeof(char*) * librarie.nrCarti);

		for (int i = 0; i < librarie.nrCarti; i++)
		{
			char buffer[100];
			fscanf(f, "%s", buffer);

			librarie.carti[i] =
				(char*)malloc(strlen(buffer) + 1);

			strcpy(librarie.carti[i], buffer);
		}
	}
	else
	{
		librarie.carti = NULL;
	}

	return librarie;
}

Nod* citireFisier(const char* numeFisier)
{
	Nod* arbore = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f)
	{
		int dim = 0;
		fscanf(f, "%d", &dim);

		for (int i = 0; i < dim; i++)
		{
			Librarie l = citireLibrarie(f);
			inserareInArbore(&arbore, l);
		}

		fclose(f);
	}

	return arbore;
}

int main()
{
	Nod* arbore = citireFisier("librarii.txt");

	printf("Afisare inordine:\n");
	afisareArboreInordine(arbore);

	printf("\nNumar total carti: %d\n", calculNrCarti(arbore));

	return 0;
}