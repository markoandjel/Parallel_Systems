// Decembarski 2023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<omp.h>
#include <stdio.h>
#define N 10
int main()
{
    int A[N][N],B[N][N],C[N][N],C_new[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = i*N+j;
            B[i][j] = i*N+j;
        }
    }
    //sek
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int t = 0; t < N; t++)
            {
                C[i][j] += A[i][t]*B[t][j];
            }
        }
    }
    printf_s("Ovo je matrica C koja je sekvencijalno dobijena\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf_s("%d ", C[i][j]);
        }
        printf_s("\n");
    }

    //paralelni deo
    int i,j, t;
        #pragma omp parallel for private(j,t)
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                C_new[i][j] = 0;
                for (t = 0; t < N; t++)
                {
                    C_new[i][j] += A[i][t] * B[t][j];
                }
            }
        }
    
    printf_s("Ovo je matrica C koja je paralelno dobijena\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf_s("%d ", C_new[i][j]);
        }
        printf_s("\n");
    }

}
