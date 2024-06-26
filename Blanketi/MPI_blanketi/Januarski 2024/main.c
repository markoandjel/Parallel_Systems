#include "mpi.h"
#include "stdio.h"
#include "math.h"

#define n 3
#define k 7 

void main(int argc, char* argv[])
{
    int A[k][n],b[n],d[k],rank,p,master=0;
    int* local_a;
    int* local_d;


    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    int v=(int)pow(2,rank);
    local_a=calloc(v*n,sizeof(int));
    local_d=calloc(v,sizeof(int));
    /*for(int i=0;i<v;i++)
    {
        local_a[i]=calloc(n,sizeof(int));
    }*/
    if(rank==master)
    {
        for(int i=0;i<k;i++)
        {
            for(int j=0;j<n;j++)
            {
                A[i][j]=i+j;
            }
        }
        for(int i=0;i<n;i++)
        {
            b[i]=i*3+2;
            local_a[i]=A[0][i];
        }

        for(int i=1;i<p;i++)
        {
            MPI_Send(&A[(int)pow(2,i)-1][0],n*(int)pow(2,i),MPI_INT,i,0,MPI_COMM_WORLD);
        }
    }   
    else
    {
        MPI_Recv(&local_a[0],n*v,MPI_INT,master,0,MPI_COMM_WORLD,&status);        
    }
    MPI_Bcast(&b,n,MPI_INT,0,MPI_COMM_WORLD);
    
    for(int i=0;i<(int)pow(2,rank);i++)
    {
        int sum=0;
        for(int j=0;j<n;j++)
        {
            sum+=local_a[i*n+j]*b[j];
            //printf_s("Element matrice local_a: %d,%d\n",local_a[i*n+j],rank);
        }
        //printf_s("Sum:%d,i:%d,rank:%d\n",sum,i,rank);
        local_d[i]=sum;
    }
    /*printf_s("Lokal_d\n");
    for(int i=0;i<(int)pow(2,rank);i++)
    {
        printf_s("Rank:%d, value:%d\n",rank,local_d[i]);
    }*/
    master=p-1;
    MPI_Bcast(&master,1,MPI_INT,0,MPI_COMM_WORLD);
    if(rank==master)
    {
        for(int i=0;i<p-1;i++)
        {
            MPI_Recv(&d[(int)pow(2,i)-1],(int)pow(2,i),MPI_INT,i,1,MPI_COMM_WORLD,&status);
        }
        for(int i=0;i<v;i++)
        {
            d[k-v+i]=local_d[i];
        }

        printf_s("Niz d:\n");
        for(int i=0;i<k;i++)
        {
            printf_s("%d\n",d[i]);
        }
    }
    else
    {
        MPI_Send(&local_d[0],v,MPI_INT,master,1,MPI_COMM_WORLD);
    }

    MPI_Finalize();
}