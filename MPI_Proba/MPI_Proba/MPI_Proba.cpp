// MPI_Proba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "mpi.h"
#define n 10

int main(int argc, char* argv[])
{
	int my_rank;
	float A[n][n];
	MPI_Status status;
	MPI_Datatype column;
	int i,j;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Type_vector(n, 1, n, MPI_FLOAT, &column);
	MPI_Type_commit(&column);

	if (my_rank == 0)
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				A[i][j] = (float)i;
		MPI_Send(&(A[0][0]), 1, column,1, 0, MPI_COMM_WORLD);
	}	
	else
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				A[i][j] = 0.0;
		MPI_Recv(&(A[0][0]), n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
		for (j = 0; j < n; j++)
			printf("%3.1f ", A[0][j]);
		printf("\n");
	}
	MPI_Finalize();
}

