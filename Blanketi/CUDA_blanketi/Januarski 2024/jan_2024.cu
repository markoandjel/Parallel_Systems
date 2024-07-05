#include <cuda_runtime.h>
#include<stdio.h>
#include <math.h>
#include "device_launch_parameters.h"
#define N 128
#define FLT_MAX 3.402823466e+38F
typedef struct pov
{
	float rastojanje; int index;
}pov;
typedef struct point
{
	float x, y;
}point;
 __device__ float rastojanje(point* t1, point* t2)
{
	return sqrtf((t1->x - t2->x) * (t1->x - t2->x) + (t1->y - t2->y) * (t1->y - t2->y));
}

 __device__ void findMin(pov* dev_rastind)
 {
	for (unsigned int offset = blockDim.x / 2; offset > 0; offset >>= 1)
	{
		if (threadIdx.x < offset) {
			if (dev_rastind[threadIdx.x].rastojanje > dev_rastind[threadIdx.x + offset].rastojanje)
			{
				dev_rastind[threadIdx.x].rastojanje = dev_rastind[threadIdx.x + offset].rastojanje;
				dev_rastind[threadIdx.x].index = dev_rastind[threadIdx.x + offset].index;
			}
		}
		__syncthreads();
	}
 }

__global__ void kernel(point* dev_niz,point*tacka,point* dev_blizu)
{
	__shared__ float rastojanja[N];
	__shared__ pov dev_rastind[N];
	__shared__ int indeksi[10];
	if (threadIdx.x < 10)
	{
		indeksi[threadIdx.x] = -1;
	}
	rastojanja[threadIdx.x] = rastojanje(tacka, &dev_niz[threadIdx.x]);
	/*dev_rastind[threadIdx.x].rastojanje = rastojanja[threadIdx.x];
	dev_rastind[threadIdx.x].index = threadIdx.x;*/
	__syncthreads();

	for (int i = 0; i < 10; i++)
	{
		dev_rastind[threadIdx.x].rastojanje = rastojanja[threadIdx.x];
		dev_rastind[threadIdx.x].index = threadIdx.x;
		__syncthreads();
		if (threadIdx.x < 10)
		{
			if (indeksi[threadIdx.x] > -1)
			{
				dev_rastind[indeksi[threadIdx.x]].rastojanje = FLT_MAX;
			}
		}
		__syncthreads();

		findMin(dev_rastind);
		__syncthreads();
		if(threadIdx.x==0)
		indeksi[i] = dev_rastind[0].index;
	}
	__syncthreads();
	if (threadIdx.x<10)
	{
		dev_blizu[threadIdx.x].x = dev_niz[indeksi[threadIdx.x]].x;
		dev_blizu[threadIdx.x].y = dev_niz[indeksi[threadIdx.x]].y;
	}

}
int main()
{
	point niz[N],blizu[10];
	point tacka;
	for (int i = 0; i < N; i++)
	{
		niz[i].x = i + 1.54;
		niz[i].y = i - 1.32;
	}
	//tacka.x = 5.543;
	//tacka.y = 3.658;
	printf_s("Unesite x:\n");
	scanf_s("%f", &tacka.x);
	printf_s("Unesite y:\n");
	scanf_s("%f", &tacka.y);

	point* dev_niz,*dev_tacka,*dev_blizu;
	cudaMalloc((void**)&dev_niz, N * sizeof(point));
	cudaMalloc((void**)&dev_tacka, sizeof(point));
	cudaMalloc((void**)&dev_blizu, 10*sizeof(point));
	cudaMemcpy(dev_niz, niz, N * sizeof(point), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_tacka, &tacka, sizeof(point), cudaMemcpyHostToDevice);

	kernel<<<1,N>>>(dev_niz, dev_tacka, dev_blizu);

	cudaMemcpy(blizu, dev_blizu, 10*sizeof(point), cudaMemcpyDeviceToHost);
	cudaFree(dev_niz);
	cudaFree(dev_tacka);
	cudaFree(blizu);
	for (int i = 0; i < 10; i++)
	{
		printf_s("%f,%f\n", blizu[i].x, blizu[i].y);
	}
	return 0;
}