/*
Napisati MPI program kojim se matrice Amxn i Bnxk, inicijalizovane u master procesu, množe i
rezultat se smešta u matricu C. Svakom procesu šalje se cela matrica A i po k/l kolona matrice B (l
je broj procesa, k je deljivo sa l) i svi procesi učestvuju u izračunavanju. Slanje k/l kolona matrice
B obavlja se odjednom, direktno iz matrice B. Matricu C prikazati iz master procesa. Zadatak
rešiti korišćenjem grupnih operacija i izvedenih tipova podataka.
*/

#include <iostream>
#include "mpi.h"

int main(int argc,char** argv)
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 8, m = 4, k = 12, l = 4;

    int A[m][n], B[n][k], C[m][k], local_B[n][k / l], local_C[m][k / l],i,j,v;

    if (rank == 0)
    {
        printf("\nA:\n");
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                A[i][j] = i * m + j;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
           

        printf("\nB:\n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                B[i][j] = i+j/2;
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
            
    }

    MPI_Bcast(&A[0][0], m * n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Datatype temp, column_send, column_recv;
    MPI_Type_vector(n, k / l, k, MPI_INT, &temp);
    MPI_Type_create_resized(temp, 0, (k / l) * sizeof(int), &column_send);
    MPI_Type_commit(&column_send);

    MPI_Scatter(&B[0][0], 1, column_send, &local_B[0][0], n * (k / l), MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < k / l; j++)
        {
            local_C[i][j] = 0;
            for (v = 0; v < n; v++)
            {
                local_C[i][j] += A[i][v] * local_B[v][j];
            }
        }

    }

    MPI_Type_vector(m, k / l, k, MPI_INT, &temp);
    MPI_Type_create_resized(temp, 0, (k / l) * sizeof(int), &column_recv);
    MPI_Type_commit(&column_recv);

    MPI_Gather(&local_C[0][0], m * (k / l), MPI_INT, &C[0][0], 1, column_recv, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nC:\n");
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < k/l; j++)
                printf("%d ", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}