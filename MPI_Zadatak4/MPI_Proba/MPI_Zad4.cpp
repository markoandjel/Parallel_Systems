// MPI_Proba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
	int rank;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	

	MPI_Finalize();
	return 0;
}

