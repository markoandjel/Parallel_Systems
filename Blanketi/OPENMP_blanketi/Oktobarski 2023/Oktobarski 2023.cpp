// Oktobarski 2023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <omp.h>
#define N 100
#define p 1
int main()
{
	int m = 2, i, j, a[N], b[N*N*p+2];

	for (int i = 0; i < N; i++) { a[i] = i; };

	printf_s("\n");
	for (int i = 0; i < N * N * p + 2; i++) b[i] = i;  //printf_s("%d ", b[i]);};

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			a[j] += b[m];
			m += p;
		}
	}

	printf_s("\n");
	for (int i = 0; i < N; i++) { printf_s("%d ", a[i]); };
	printf("\n%d\n", m);
	printf_s("\nParalelni deo\n");
	for (int i = 0; i < N; i++) a[i] = i;
	m = 2;
	#pragma omp parallel for reduction(+:m) private(i)
	for (j = 0; j < N; j++)
	{
		for (i = 0; i < N; i++)
		{
			//#pragma omp critical
			a[j] += b[((i*N+j)*p+2)];
			m += p;
		}
	}
	printf("\n%d\n", m);
	for (int i = 0; i < N; i++) { printf_s("%d ", a[i]); };
}
