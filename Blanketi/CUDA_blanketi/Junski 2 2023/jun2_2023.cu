#include <cuda_runtime.h>
#include "cuda.h"
#include <stdio.h>
#include <stdlib.h>
#include "device_launch_parameters.h"
using namespace std;


__device__ void racunaj(float* dev_niz, float* dev_b)
{
	dev_b[blockDim.x * blockIdx.x + threadIdx.x] = (dev_niz[threadIdx.x] * dev_niz[threadIdx.x+1] * dev_niz[threadIdx.x+2]) /
		(dev_niz[threadIdx.x] + dev_niz[threadIdx.x + 1] + dev_niz[threadIdx.x + 2]);
}
__global__ void kernel(float* dev_a, float*dev_b)
{
	__shared__ float dev_niz[256];
	dev_niz[threadIdx.x] = dev_a[blockDim.x * blockIdx.x + threadIdx.x];
	__syncthreads();
	if (threadIdx.x < blockDim.x - 2)
	{
		racunaj(dev_niz, dev_b);
	}
	__syncthreads();
}

int main()
{
	int n = 258;
	float* a = (float*)calloc(n, sizeof(float));
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}

	float* b = (float*)calloc(n-2, sizeof(float));
	float* dev_a,*dev_b;
	cudaMalloc((void**)&dev_a, n*sizeof(float));
	cudaMalloc((void**)&dev_b, (n-2)*sizeof(float));
	cudaMemcpy(dev_a, a, n*sizeof(float), cudaMemcpyHostToDevice);
	dim3 blockDim(n/256+1, 1, 1);
	dim3 threadDim((n/blockDim.x)+1, 1, 1);
	kernel <<<blockDim,threadDim>>> (dev_a,dev_b);
	cudaMemcpy(b, dev_b, (n - 2)*sizeof(float), cudaMemcpyDeviceToHost);
	for (int i = 0; i < n - 2; i++)
	{
		printf_s("%d,%f \n", i,b[i]);
	}
	cudaFree(dev_b);
	cudaFree(dev_a);
	free(a);
	free(b);
}