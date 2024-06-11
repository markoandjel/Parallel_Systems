/*
Napisati MPI program koji pronalazi minimalnu vrednost u segmentu kvadratne matrice reda n
koga čine kolone matrice sa parnim indeksom (j=0,2,4,...). Smatrati da je n parno. Matrica je
inicijalizovana u procesu P0 i konačni rezultat treba se izračunati u ovom procesu. Svaki proces
dobija elemente kolona s parnim indeksom iz odgovarajućih n/m vrsta (m je broj procesa, n je
deljivo sa m) i nalazi lokalni minimum. Izračunati i prikazati globalni minimum u traženom
segmentu matrice. Zadatak realizovati korišćenjem isključivo grupnih operacija i izvedenih tipova
podataka.
*/



#include <iostream>
#include "mpi.h"
int main(int argc,char** argv)
{
    int rank;

    const int n = 16,m=4;
    int A[n][n], B[n / m][n / 2],i,j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("\n Matrica A:\n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                A[i][j] = i + j * n;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        fflush(stdout);
    }

    MPI_Datatype temp_type, send_type;
    MPI_Type_vector(n / 2, 1, 2, MPI_INT, &temp_type);
    MPI_Type_create_resized(temp_type, 0, n * sizeof(int), &send_type);
    MPI_Type_commit(&send_type);

    MPI_Scatter(&A[0][0], n / m, send_type, &B[0][0], (n / m)*(n/2),MPI_INT,0,MPI_COMM_WORLD);

    printf("\n Matrica B u procesu P%d\n",rank);
    for (i = 0; i < n/m; i++)
    {
        for (j = 0; j < n/2; j++)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    struct { int min; int rank; } local_min{},min{}; //pomocna struktura za pamcenje minimuma i ranka

    local_min.min = B[0][0];
    local_min.rank = rank;

    for(i=0;i<n/m;i++)
        for (j = 0; j < n / 2; j++)
        {
            if (local_min.min > B[i][j])
                local_min.min = B[i][j];
        }

    MPI_Reduce(&local_min, &min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\n ---------------------");
        printf("Globalni minimum je %d u procesu P%d", min.min, min.rank);
        printf("------------------------\n");
    }

    MPI_Finalize();
    return 0;




    
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
