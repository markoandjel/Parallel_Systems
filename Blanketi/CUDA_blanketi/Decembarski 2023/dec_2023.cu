#include <cuda_runtime.h>
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include "device_launch_parameters.h"
#define tile_size 16

__device__ void findMin(int* tajl_C)
{
	for (int i = blockDim.y / 2; i > 0; i >>=1)
	{
		if (threadIdx.y < i)
		{
			if (tajl_C[threadIdx.y * blockDim.x + threadIdx.x] > tajl_C[(threadIdx.y + i) * blockDim.x + threadIdx.x])
			{
				tajl_C[threadIdx.y * blockDim.x + threadIdx.x] = tajl_C[(threadIdx.y + i) * blockDim.x + threadIdx.x];
			}
		}
		__syncthreads();
	}
}

__device__ void izracunaj(int* tajl_A, int* tajl_B,int* dev_C,int tid)
{
	dev_C[tid] = tajl_A[threadIdx.x + threadIdx.y * blockDim.x] -
		2 * tajl_B[threadIdx.y * blockDim.y + threadIdx.x];
}
__global__ void kernel(int* dev_A, int* dev_B, int* dev_C,int n)
{
	__shared__ int tajl_A[tile_size*tile_size], tajl_B[tile_size * tile_size];
	/*int tid = blockIdx.y * blockDim.x * blockDim.y * gridDim.x +
		blockIdx.x*blockDim.x+
		threadIdx.y+threadIdx.x*blockDim.x*gridDim.x;
	int Row = blockIdx.y * tile_size + threadIdx.y;
	int Column = blockIdx.x * tile_size + threadIdx.x;
	int tid = Row * n+ + threadIdx.x;*/

	int tid = blockIdx.y * gridDim.x * blockDim.y * blockDim.x + blockDim.y * blockDim.x * blockIdx.x
		+ threadIdx.y * blockDim.x + threadIdx.x;

	tajl_A[threadIdx.y * blockDim.x + threadIdx.x] = dev_A[tid];
	tajl_B[threadIdx.y * blockDim.x + threadIdx.x] = dev_B[tid];

	__syncthreads();
	if (threadIdx.x + threadIdx.y * blockDim.x < n * n)
		izracunaj(tajl_A, tajl_B, dev_C, tid);
	__syncthreads();

}

__global__ void kernel2(int* dev_C, int* dev_v_extended)
{
	__shared__ int tajl_C[tile_size * tile_size];
	int tid = blockIdx.y * gridDim.x * blockDim.y * blockDim.x + blockDim.x * blockIdx.x
		+ threadIdx.y * blockDim.x * gridDim.x + threadIdx.x;

	tajl_C[threadIdx.y * blockDim.x + threadIdx.x] = dev_C[tid];
	__syncthreads();
	findMin(tajl_C);
	__syncthreads();
	if (threadIdx.y == 0)
	{
		int p = blockIdx.y * blockDim.x * gridDim.x + gridDim.x * blockIdx.x + threadIdx.x;
		dev_v_extended[p] = tajl_C[threadIdx.x];
	}
}
__global__ void kernel3(int* dev_v_extended)
{
	for (int i = blockDim.x / 2; i > 0; i >>= 1)
	{
		if (threadIdx.y < i)
		{
			if (dev_v_extended[threadIdx.y*blockDim.x*gridDim.x+blockIdx.x*blockDim.x+threadIdx.x] 
				> dev_v_extended[threadIdx.y * blockDim.x * gridDim.x + blockIdx.x * blockDim.x + threadIdx.x + i*blockDim.x])
			{
				dev_v_extended[threadIdx.y * blockDim.x * gridDim.x + blockIdx.x * blockDim.x + threadIdx.x] =
					dev_v_extended[threadIdx.y * blockDim.x * gridDim.x + blockIdx.x * blockDim.x + threadIdx.x + i * blockDim.x];
			}
		}
		__syncthreads();
	}
}

int main()
{
	int n = 32;
	int* A=(int*)calloc(n*n,sizeof(int));
	int* B=(int*)calloc(n * n, sizeof(int));
	int* C = (int*)calloc(n * n, sizeof(int));
	int* v = (int*)calloc(n, sizeof(int));
	int* dev_A,*dev_B,*dev_C,* dev_v_extended;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A[i * n + j] = i * n + 3*j;
			B[i * n + j] = i * n + j;
		}
	}
	cudaMalloc((void**)&dev_A, n*n*sizeof(int));
	cudaMalloc((void**)&dev_B, n * n * sizeof(int));
	cudaMalloc((void**)&dev_C, n * n * sizeof(int));
	cudaMalloc((void**)&dev_v_extended, n* (int)ceil(n / tile_size) *sizeof(int));

	cudaMemcpy(dev_A, A, n * n * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_B, B, n * n * sizeof(int), cudaMemcpyHostToDevice);
	dim3 blok(tile_size,tile_size,1);
	dim3 grid((int)ceil(n/tile_size),(int)ceil(n/tile_size),1);
	kernel <<<grid,blok>>> (dev_A,dev_B,dev_C,n);
	kernel2 <<<grid,blok>>> (dev_C, dev_v_extended);
	kernel3 <<<dim3((int)ceil(n/tile_size),1,1), dim3(tile_size*tile_size,1,1) >> > (dev_v_extended);
	cudaMemcpy(C, dev_C, n * n * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(v, dev_v_extended, n * sizeof(int), cudaMemcpyDeviceToHost);

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

	cudaFree(dev_A);
	cudaFree(dev_B);
	cudaFree(dev_C);
	cudaFree(dev_v_extended);
	free(A);
	free(B);
	free(C);
	free(v);
	return 0;
}