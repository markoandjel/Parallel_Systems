#include <cuda_runtime.h>
#include "cuda.h"
#include <stdio.h>
#include <stdlib.h>
#include "device_launch_parameters.h"
#include "math.h"

#define block_size 256

__global__ void kernel(int* A_dev, int* B_dev, int* C_dev,int n)
{
	int ind = blockIdx.x * blockDim.x + threadIdx.x;
	if (ind < n * n)
	{
		int r = ind / n;
		int c = ind % n;
		int ind2 = c * n + r;
		if (A_dev[ind] > B_dev[ind2])
			C_dev[ind] = A_dev[ind];
		else C_dev[ind] = B_dev[ind2];
	}
}


__global__ void kernel2(int* C_dev,int n,int* v_dev)
{
	__shared__ int local_v[block_size];
	if (threadIdx.x < n)
	{
		local_v[threadIdx.x] = C_dev[n * blockIdx.x + threadIdx.x];
	}
	__syncthreads();

	if (threadIdx.x >= n) local_v[threadIdx.x] = INT_MAX;

	__syncthreads();
	for (int s = blockDim.x / 2; s > 0; s /= 2)
	{
		if (threadIdx.x < s)
		{
			if (local_v[threadIdx.x] > local_v[threadIdx.x + s])
			{
				local_v[threadIdx.x] = local_v[threadIdx.x + s];
			}
		}
		__syncthreads();
	}
	if (threadIdx.x == 0)
	{
		v_dev[blockIdx.x] = local_v[0];
	}

}


int main()
{
	int n = 50;
	int* A = (int*)calloc(n * n, n * n * sizeof(int));
	int* B = (int*)calloc(n * n, n * n * sizeof(int));
	int* C = (int*)calloc(n * n, n * n * sizeof(int));
	int* v = (int*)calloc(n, n * sizeof(int));

	int* A_dev = (int*)calloc(n * n, n * n * sizeof(int));
	int* B_dev = (int*)calloc(n * n, n * n * sizeof(int));
	int* C_dev = (int*)calloc(n * n, n * n * sizeof(int));
	int* v_dev = (int*)calloc(n, n * sizeof(int));


	for (int i = 0; i < n * n; i++)
	{
		A[i] = i;
		B[i] = i + 1;
	}

	cudaMalloc((void**)&A_dev, n * n * sizeof(int));
	cudaMalloc((void**)&B_dev, n * n * sizeof(int));
	cudaMalloc((void**)&C_dev, n * n * sizeof(int));
	cudaMalloc((void**)&v_dev, n * sizeof(int));

	cudaMemcpy(A_dev, A, n * n * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(B_dev, B, n * n * sizeof(int), cudaMemcpyHostToDevice);

	kernel <<< block_size, block_size >>> (A_dev, B_dev, C_dev,n);
	kernel2 << <block_size, block_size >> > (C_dev, n, v_dev);



	cudaMemcpy(C, C_dev, n * n * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(v, v_dev, n * sizeof(int), cudaMemcpyDeviceToHost);

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
		printf_s("%d ", v[i]);
	}



	free(A); free(B); free(C); free(v);
	cudaFree(A_dev); cudaFree(B_dev); cudaFree(C_dev); cudaFree(v_dev);
	return 0;
}