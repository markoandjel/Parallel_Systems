// Oktobarski 2 2023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <omp.h>
#define N 10
int main()
{
	int rok[N], rok2[N];
	int a = 0;
	/*omp_set_num_threads(3);
	#pragma omp parallel
	{
		#pragma omp task
		printf_s("A\n");

		#pragma omp task
		printf_s("B\n");
	}*/
	for (int i = 0; i < N; i++) {
		rok[i] = i;
	}
	for (int i = N - 1; i > 1; i--)
	{
		a += 2;
		rok[i] = rok[i - 1] + a;
	}

	for (int i = 0; i < N; i++) printf_s("%d ", rok[i]);

	
	for (int i = 0; i < N; i++) rok[i] = i;
	int i;
	a = 0;
	#pragma omp parallel for shared(rok,rok2) 
		for(i = N-1; i>0; i--)
		{
			rok2[i] = rok[i-1];
		}
		a += 2;
		#pragma omp parallel for shared(rok,rok2) reduction(+:a)
		for (i = N - 1; i > 1; i--)
		{
			rok[i] = rok2[i] + a;
			a += 2;
		}
	printf_s("\nParalelno\n");
	for (int i = 0; i < N; i++) printf_s("%d ", rok[i]);
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
