#include <cuda_runtime.h>
#include "cuda.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define N 10 
#define Pi 3.14152 

typedef struct krug{
	float x,y,r;
}krug;

__device__ float povrsina(float r)
{
	return r * r * Pi;
}

__global__ void kernel(struct krug* dev_krugovi,struct krug* dev_rezultat)
{
	__shared__ float povrsine[N];
	povrsine[threadIdx.x] = povrsina(dev_krugovi[threadIdx.x].r);
	__syncthreads();
	if (threadIdx.x == 0)
	{
		float pom_max = povrsine[0];
		int index_max = 0;
		float pom = povrsine[0];
		int index = 0;
		for (int i = 1; i < N; i++)
		{
			if (povrsine[i] > pom_max)
			{
				pom = pom_max;
				index = index_max;
				pom_max = povrsine[i];
				index_max = i;
			}
			else if(povrsine[i]>pom)
			{
				pom = povrsine[i];
				index = i;
			}
		}
		dev_rezultat[0].r = dev_krugovi[index].r;
		dev_rezultat[0].x = dev_krugovi[index].x;
		dev_rezultat[0].y = dev_krugovi[index].y;

		dev_rezultat[1].r = dev_krugovi[index_max].r;
		dev_rezultat[1].x = dev_krugovi[index_max].x;
		dev_rezultat[1].y = dev_krugovi[index_max].y;
	}
}

int main()
{
	krug rezultat[2];
	krug krugovi[N];
	for (int i = 0; i < N; i++)
	{
		krugovi[i].x = i * N;
		krugovi[i].y = i + N;
		krugovi[i].r = i + 1;
	}
	krug* dev_krugovi;
	krug* dev_rezultat;
	//printf_s("%d,%d\n", (int)sizeof(struct krug),sizeof(double));
	cudaMalloc((void**)&dev_krugovi, N * sizeof(krug));
	cudaMemcpy(dev_krugovi, &krugovi[0], N * sizeof(krug), cudaMemcpyHostToDevice);
	cudaMalloc((void**)&dev_rezultat, 2*sizeof(krug));
	kernel <<<1, N>>> (dev_krugovi, dev_rezultat);

	cudaMemcpy(&rezultat, dev_rezultat, 2*sizeof(krug), cudaMemcpyDeviceToHost);
	cudaFree(dev_krugovi);
	cudaFree(dev_rezultat);
	printf_s("%f,%f:%f \n", rezultat[0].x, rezultat[0].y, rezultat[0].r);
	printf_s("%f,%f:%f \n",rezultat[1].x, rezultat[1].y, rezultat[1].r);
}