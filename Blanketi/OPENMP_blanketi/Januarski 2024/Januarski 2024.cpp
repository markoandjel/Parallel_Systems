// Januarski 2024.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <omp.h>
#include <stdio.h>
#define M 2
int main()
{
	int A[M * M];
	for (int i = 0; i < M * M; i++)
	{
		A[i] = i;
	}

	//A[2] = 100;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{

			printf_s("%d ", A[i*M+j]);
		}
		printf_s("\n");
	}

	int pom = A[0];
	for (int i = 0; i < M * M; i++)
	{
		if (A[i] > pom) pom = A[i];
	}
	printf("%d", pom);

	int max1=A[0];

	#pragma omp parallel 
	{
		int tid = omp_get_num_threads();
		#pragma omp for reduction(max : max1)
		for (int i = 0; i < M*M; i++)
			if (A[i] > max1)max1 = A[i];	
	}

	printf("\n%d \n", max1);
}
