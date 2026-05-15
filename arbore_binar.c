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

void afisareArborePostordine(Nod* rad) // SDR
{
	if (rad)
	{
		afisareArborePostordine(rad->st);
		afisareArborePostordine(rad->dr);
		afisareLibrarie(rad->info);
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

int numarNoduri(Nod* rad)
{
	if (rad)
	{
		return 1 + numarNoduri(rad->st) + numarNoduri(rad->dr);
	}
	return 0;
}

int inaltimeArbore(Nod* rad)
{
	if (rad == NULL)
	{
		return 0;
	}

	int inaltimeStanga = inaltimeArbore(rad->st);
	int inaltimeDreapta = inaltimeArbore(rad->dr);

	if (inaltimeStanga > inaltimeDreapta)
	{
		return 1 + inaltimeStanga;
	}
	else
	{
		return 1 + inaltimeDreapta;
	}
}

Nod* cautareDupaId(Nod* rad, int id)
{
	if (rad == NULL)
	{
		return NULL;
	}

	if (rad->info.id == id)
	{
		return rad;
	}

	if (id < rad->info.id)
	{
		return cautareDupaId(rad->st, id);
	}
	else
	{
		return cautareDupaId(rad->dr, id);
	}
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

			librarie.carti[i] = (char*)malloc(strlen(buffer) + 1);
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

void dezalocareLibrarie(Librarie* librarie)
{
	for (int i = 0; i < librarie->nrCarti; i++)
	{
		free(librarie->carti[i]);
	}

	free(librarie->carti);
	librarie->carti = NULL;
	librarie->nrCarti = 0;
}

void dezalocareArbore(Nod** rad)
{
	if (*rad)
	{
		dezalocareArbore(&((*rad)->st));
		dezalocareArbore(&((*rad)->dr));

		dezalocareLibrarie(&((*rad)->info));
		free(*rad);
		*rad = NULL;
	}
}

int main()
{
	Nod* arbore = citireFisier("librarii.txt");

	printf("Afisare inordine:\n");
	afisareArboreInordine(arbore);

	printf("\nAfisare preordine:\n");
	afisareArborePreordine(arbore);

	printf("\nAfisare postordine:\n");
	afisareArborePostordine(arbore);

	printf("\nNumar total carti: %d\n", calculNrCarti(arbore));
	printf("Numar librarii: %d\n", numarNoduri(arbore));
	printf("Inaltime arbore: %d\n", inaltimeArbore(arbore));

	int idCautat = 4;
	Nod* gasit = cautareDupaId(arbore, idCautat);

	if (gasit)
	{
		printf("\nLibraria cu id-ul %d a fost gasita:\n", idCautat);
		afisareLibrarie(gasit->info);
	}
	else
	{
		printf("\nNu exista librarie cu id-ul %d.\n", idCautat);
	}

	dezalocareArbore(&arbore);

	return 0;
}