// MPI_Proba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
	int rank;
	struct { int a; double b; } value;
	MPI_Datatype mystruct;
	int blocklens[2];
	MPI_Aint indices[2];
	MPI_Datatype old_types[2];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	blocklens[0] = 1;
	blocklens[1] = 1;
	old_types[0] = MPI_INT;
	old_types[1] = MPI_DOUBLE;

	MPI_Get_address(&value.a,&indices[0]);
	MPI_Get_address(&value.b, &indices[1]);

	indices[1] = indices[1] - indices[0];
	indices[0] = 0;
	MPI_Type_create_struct(2, blocklens, indices, old_types, &mystruct);
	MPI_Type_commit(&mystruct);

	if (rank == 0)
		scanf_s("%d %lf", &value.a, &value.b);
	MPI_Bcast(&value, 1, mystruct, 0, MPI_COMM_WORLD);	
	printf("Process %d got %d and %lf\n", rank, value.a, value.b);

	MPI_Finalize();
	return 0;
}

