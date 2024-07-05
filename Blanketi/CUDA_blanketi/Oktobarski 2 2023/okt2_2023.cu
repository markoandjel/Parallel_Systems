#include <cuda_runtime.h>
#include "cuda.h"
#include <stdio.h>
#include <stdlib.h>
#include "device_launch_parameters.h"
#include "math.h"
using namespace std;

#define blockSize 32


__global__ void kernel(int* dev_A,int*dev_B,int*dev_C,int n)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < n * n)
	{
		int row = i / n;
		int col = i % n;
		int ind = col * n + row;
		dev_C[i] = dev_A[i] < dev_B[ind] ? dev_A[i] : dev_B[ind];
		i += blockIdx.x * blockDim.x;
	}
}

__global__ void kernel2(int* dev_C, float* dev_rez,int n)
{
	__shared__ int suma[blockSize];
	//int slic = n % blockDim.x == 0 ? (int) n / blockDim.x : (int)n/blockDim.x + 1;
	int brojBloka = blockIdx.x;
	while (brojBloka<n)
	{
		/*int start = threadIdx.x * slic;
		int end = start + slic;
		if (end > n) end = n;
		for (int i = start; i < end; i++)
		{
			if (dev_C[brojBloka * n + i] < localMin) localMin = dev_C[brojBloka * n + i];
		}
		*/
		suma[threadIdx.x] = dev_C[threadIdx.x * blockDim.x + blockIdx.x * blockDim.x];
		__syncthreads();
		for (int s = n / 2; s > 0; s /= 2)
		{
			if (threadIdx.x < s)
			{
				suma[threadIdx.x] += suma[threadIdx.x + s];
			}
			__syncthreads();
		}
		if (threadIdx.x == 0)
		{
			dev_rez[brojBloka] = (float)suma[0]/n;
		}
		brojBloka += gridDim.x;
	}
}

int main()
{
	int n = 16;
	int* A = (int*)calloc(n * n, n * n * sizeof(int));
	int* B = (int*)calloc(n * n, n * n * sizeof(int));
	int* C = (int*)calloc(n * n, n * n * sizeof(int));
	float* rez = (float*)calloc(n, n * sizeof(float));
	for (int i = 0; i < n * n; i++)
	{
		A[i] = i + 1;
		B[i] = i+2;
	}
	int* dev_A, * dev_B, * dev_C;
	float*dev_rez;
	cudaMalloc((void**)&dev_A, n * n * sizeof(int));
	cudaMalloc((void**)&dev_B, n * n * sizeof(int));
	cudaMalloc((void**)&dev_C, n * n * sizeof(int));
	cudaMalloc((void**)&dev_rez,n * sizeof(float));

	cudaMemcpy(dev_A, A, n * n * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_B, B, n * n * sizeof(int), cudaMemcpyHostToDevice);

	kernel <<<blockSize, blockSize >> > (dev_A,dev_B,dev_C,n);
	cudaMemcpy(C, dev_C, n * n * sizeof(int), cudaMemcpyDeviceToHost);

	kernel2 << <blockSize, n >> > (dev_C, dev_rez,n);
	cudaMemcpy(rez, dev_rez, n * sizeof(float), cudaMemcpyDeviceToHost);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf_s("%d ", C[i * n + j]);
		}
		printf_s("\n");
	}

	for (int i = 0; i < n; i++)
	{
		printf_s("%.4f ", rez[i]);
	}

	free(A); free(B); free(C);
}