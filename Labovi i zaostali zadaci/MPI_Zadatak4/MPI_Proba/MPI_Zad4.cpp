// MPI_Proba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "mpi.h"
#define m 2
#define n 3
#define k 4

int main(int argc, char* argv[])
{
	int a[m][n], b[n][k], c[m][k], local_c[m][k], niza[m], nizb[k], rank, i, j, p;

	MPI_Datatype vector, column;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	MPI_Type_vector(m, 1, n, MPI_INT, &vector);
	MPI_Type_commit(&vector);
	MPI_Type_create_resized(vector, 0, m*n * sizeof(int), &column);
	MPI_Type_commit(&column);

	if (rank == 0)
	{
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				a[i][j] = i + j;
		for (i = 0; i < n; i++)
			for (j = 0; j < k; j++)
				b[i][j] = 1+j-i;
	}
	MPI_Scatter(a, 1, column, niza, m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, k, MPI_INT, nizb, k, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = 0; i < m; i++)
		for (j = 0; j < k; j++)
			local_c[i][j] = niza[i] * nizb[j];
	for (i = 0; i < m; i++)
		for (j = 0; j < k; j++)
			c[i][j] = 0;

	MPI_Reduce(&local_c[0][0], &c[0][0], m * k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (j = 0; j < k; j++)
			printf("%d ", c[i][j]);
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}

