//Napisati MPI program kojim se kreira dvodimenzionalna Cartesian struktura sa n vrsta i m
//kolona.Svaki proces računa proizvod identifikatora njegovog gornjeg i donjeg suseda.
//Obezbediti da se u master procesu na kraju programa nađe suma izračunatih proizvoda.
//Ilustrovati raspored procesa i diskutovati dobijeno rešenje u zavisnosti od periodičnosti
//dimenzija.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm cart_comm;
    int cart_rank, upper, lower, part_prod = 0, sum;
    const int dimensions = 2,
        n = 3,
        m = 2,
        dims[dimensions] = { n, m },
        periods[dimensions] = { 1, 0 };

    MPI_Cart_create(MPI_COMM_WORLD, dimensions, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_shift(cart_comm, 0, 1, &upper, &lower);

    part_prod= upper * lower;
    MPI_Reduce(&part_prod, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!rank)
        printf("Total sum is %d\n", sum);

    MPI_Finalize();
    return 0;
}
