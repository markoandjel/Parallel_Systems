// Junski 2024.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <omp.h>
#include <stdio.h>
#define N 10
int main()
{
	float d = 42;
	int a[N], a_new[N], z[N];// , z[N - 2], b[N - 2], c[N - 2];
	for (int i = 0; i < N; i++)
	{
		a[i] = i; //b[i] = i + 1; c[i] = i - 1; z[i] = i * i
		z[i] = i+1;
	}
	#pragma omp parallel for
	for (int i = N-2; i >=0; i--)
	{
		a[i] = -a[i+1];
	}
	#pragma omp parallel for reduction(/:d)
	for (int i = N - 2; i >=0; i --)
	{
		d = d / z[i];
	}
	printf_s("Paralelno\n");
	for (int i = 0; i < N; i++)
	{
		printf_s("%d ", a[i]);
	}
	printf_s("\n%f",d);
	printf_s("\n");
	printf_s("Sekvencijalno\n");
	for (int i = 0; i < N; i++)
	{
		a[i] = i; //b[i] = i + 1; c[i] = i - 1; z[i] = i * i
		z[i] = i+1;
	}
	d = 42;

	for (int i = N - 2; i >= 0; i--)
	{
		a[i] = -a[i+1];
		d = d / z[i];
	}

	for (int i = 0; i < N; i++)
	{
		printf_s("%d ", a[i]);
	}

	printf_s("\n%f", d);

	return 0;
}
