// Januarski 2023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <omp.h>

#define N 5
#define M 4
int main()
{
	int x = 0;
	int a[N][M],b[N][M];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			a[i][j] = i * N + j;
			b[i][j] = i * N + j;
		}
	}

	/*for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			printf_s("%d ",a[i][j]);
		}
		printf_s("\n");
	}*/

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			printf_s("%d ", b[i][j]);
		}
		printf_s("\n");
	}
	printf_s("--------------------------\n");
	for (int i = 2; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			x = x + a[i][j];
			b[i][j] = 4 * b[i - 2][j];
		}
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			printf_s("%d ", b[i][j]);
		}
		printf_s("\n");
	}
	printf_s("%d\n", x);
	printf_s("Paralelno\n");
	x = 0;
	int i, j;
	#pragma omp parallel for reduction(+:x) private(i)
	for (j = 0; j < M; j++)
		{
		for (i = 2; i < N; i++)
		{
			x = x + a[i][j];
			b[i][j] = 4 * b[i-2][j];
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			printf_s("%d ", b[i][j]);
		}
		printf_s("\n");
	}
	printf_s("%d\n", x);




}

