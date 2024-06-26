#include "mpi.h"
#include "stdio.h"

#define K 8 
#define N 3

#define fajl "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Junski 2 2023\\rez.dat"

int main(int argc,char*argv[])
{
    int rank, master=0,p;
    int A[K][N],b[N];
    MPI_Status status;
    MPI_Datatype blok,filetype;
    MPI_File fh;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Type_vector((int)K/p,N,p*N,MPI_INT,&blok);
    MPI_Type_commit(&blok);
    MPI_Type_vector((int)K/p,1,p,MPI_INT,&filetype);
    MPI_Type_commit(&filetype);
    int local_a[N*((int)K/p)];
    int local_d[(int)K/p];
    if (rank==master)
    {
        for(int i=0;i<K;i++)
        {
            for(int j=0;j<N;j++)
            {
                A[i][j]=i+j*4-2;
                printf_s("%d\t",A[i][j]);
            }
            printf_s("\n");
        }
        for(int i=0;i<N;i++)
        {
            b[i]=10-i;
            printf_s("%d\n",b[i]);
        }
        for(int i=1;i<p;i++)
        {
            MPI_Send(&A[i][0],1,blok,i,0,MPI_COMM_WORLD);
        }

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<(int)K/p;j++)
            {
                local_a[j*N+i]=A[j*p][i];
            }
        }
    }
    else
    {
        MPI_Recv(&local_a[0],N*(int)K/p,MPI_INT,master,0,MPI_COMM_WORLD,&status);
    }
    MPI_Bcast(&b[0],N,MPI_INT,master,MPI_COMM_WORLD);

    /*for(int i=0;i<N*(int)K/p;i++)
    {
        printf_s("value:%d,rank:%d\n",local_a[i],rank);
    }*/

    for(int i=0;i<(int)K/p;i++)
    {
        local_d[i]=0;
        for(int j=0;j<N;j++)
        {
            local_d[i]+=local_a[i*N+j]*b[j];
        }
        //printf_s("%d,%d\n",local_d[i],rank);
    }
    MPI_File_open(MPI_COMM_WORLD,fajl,MPI_MODE_WRONLY|MPI_MODE_CREATE,MPI_INFO_NULL,&fh);
    MPI_File_set_view(fh,sizeof(int)*rank,MPI_INT,filetype,"native",MPI_INFO_NULL);
    MPI_File_write_all(fh,&local_d[0],(int)K/p,MPI_INT,&status);
    MPI_Finalize();
}