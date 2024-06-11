/*Napisati MPI program koji na osnovu kvadratne matrice reda m šalje kvazidijagonale matrice.
U root procesu kreira se matrica i n - tom procesu se šalju po dve kvazidijagonale, obe na
udaljenosti n od sporedne dijagonale.Primljene dijagonale biće smeštene u drugu i treću vrstu
matrice n - tog procesa, dok će ostali elementi matrice biti popunjeni jedinicama.Korisniku
prikazati vrednosti odgovarajućih vrsta, a komunikaciju realizovati korišćenjem izvedenih tipova
podataka.*/

#include <iostream>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int m = 8;
	int A[m][m], B[m][m], i, j;
	MPI_Status status;
	MPI_Request req;

	if (rank == 0)
	{
		for (i = 0; i < m; i++)
		{
			for (j = 0; j < m; j++)
			{
				A[i][j] = i + j*m;
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);

		
	}

	for (i = 0; i < m; i++) //Postavljanje elemenata matrice B na jedinicu
		for(j=0;j<m;j++)
			B[i][j] = 1;
	MPI_Datatype send_type;
	MPI_Type_create_resized(MPI_INT, 0, (m - 1) * sizeof(int), &send_type); //sporedna dijagonala ima najvise m-1 elemenata
	MPI_Type_commit(&send_type);

	if (rank == 0)
	{
		for (i = 0; i < m/2; i++)
		{
			MPI_Isend(&A[0][m - 1 - i], m - i, send_type, i, 0, MPI_COMM_WORLD, &req);
			MPI_Isend(&A[i][m - 1], m - i, send_type, i, 0, MPI_COMM_WORLD, &req);
		}
	}

	MPI_Recv(&B[1][0], (m - rank), MPI_INT, 0, 0, MPI_COMM_WORLD, &status); //upis elemenata u B matricu
	MPI_Recv(&B[2][0], (m - rank), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

	printf("Proces P: %d\n", rank);
	for (i = 0; i < m; i++)
	{
		for (j = 0;j < m; j++)
		{
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}


	MPI_Finalize();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
