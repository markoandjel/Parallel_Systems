// MPI_Proba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "mpi.h"
#define n 10

int main(int argc, char* argv[])
{
	int p;
	int my_rank;
	float A[n][n];
	float T[n][n];
	int displacements[n];
	int block_lengths[n];
	MPI_Datatype index;
	int i, j;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	for (i = 0; i < n; i++)
	{
		block_lengths[i] = n - i;
		displacements[i] = (n + 1) * i;
	}
	MPI_Type_indexed(n, block_lengths, displacements, MPI_FLOAT, &index);
	MPI_Type_commit(&index);

	if (my_rank == 0)
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				A[i][j] = (float)i + j;
		MPI_Send(A, 1, index, 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				T[i][j] = 0.0;
		MPI_Recv(T, 1, index, 0, 0, MPI_COMM_WORLD,&status);

		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
				printf(" %4.1f ", T[i][j]);
			printf("\n");
		}			
	}
	MPI_Finalize();
}

