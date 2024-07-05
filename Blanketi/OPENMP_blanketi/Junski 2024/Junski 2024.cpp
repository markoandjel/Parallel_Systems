// Junski 2024.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <omp.h>
#include <stdio.h>
#define N 10
int main()
{
	int d = 42, a[N - 1], a_new[N - 1];// , z[N - 2], b[N - 2], c[N - 2];
	for (int i = 0; i<N-1; i++)
	{
		a[i] = i*2; //b[i] = i + 1; c[i] = i - 1; z[i] = i * i;
		a_new[i] = i*2;
	}
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = N - 2; i > 1; i -= 2)
		{
			a[i] = -a[i - 1];
		}
		#pragma omp for
		for (int i = N - 3; i > 1; i -= 2)
		{
			a[i] = -a[i - 1];
		}
	}

	for (int i = 0; i < N - 1; i++)
	{
		printf_s("%d ", a[i]);
	}
	printf_s("\n");

	for (int i = N - 2; i > 0; i--)
	{
		a_new[i] = -a_new[i-1];
	}
	for (int i = 0; i < N - 1; i++)
	{
		printf_s("%d ", a[i]);
	}
	return 0;
}
