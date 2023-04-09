//Napisati MPI program kojim se množe matrica A i B reda m.Master proces šalje svakom
//procesu po jednu vrstu prve matrice i celu drugu matricu i svi procesi učestvuju u izračunavanju.
//Za rezultujuću matricu C, naći maksimalnu vrednost elemenata svake kolone.Štampati dobijenu
//matricu, kao i nađene maksimalne vrednosti.Zadatak rešiti korišćenjem grupnih operacija.

#include <iostream>
#include "mpi.h"

int main(int argc, char** argv)
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

}
