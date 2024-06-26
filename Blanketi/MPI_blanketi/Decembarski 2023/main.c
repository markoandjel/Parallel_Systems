#include "mpi.h"
#include "stdio.h"

#define K 4
#define N 6
#define putanja "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Decembarski 2023\\B-total.dat"

int main(int argc,char*argv[])
{
    int rank, p,master=0;
    int A[K][N],b[N];
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    MPI_File fh;
    int local_a[(int)K*N/p];
    MPI_Datatype blok,filetype;
    MPI_Type_vector(K*(int)N/p,1,p,MPI_INT,&blok);
    MPI_Type_commit(&blok);
    MPI_Type_vector((int)N/p,1,p,MPI_INT,&filetype);
    MPI_Type_commit(&filetype);
    int local_b[(int)N/p];
    int local_d[K],d[K];
    struct {int rank;int value;} lmax,gmax;
    
    if(rank==master)
    {
        for(int i=0;i<K;i++)
        {
            for(int j=0;j<N;j++)
            {
                A[i][j]=i*2+j+7;
                printf_s("%d ",A[i][j]);
            }
            printf_s("\n");
        }

        /*for(int i=0;i<N;i++)
        b[i]=i+3;

        FILE* f=fopen(putanja,"w");
        fwrite(&b,sizeof(int),N,f);
        fclose(f);*/
        for(int i=1;i<p;i++)
        {
            MPI_Send(&A[0][i],1,blok,i,0,MPI_COMM_WORLD);
        }
        for(int i=0;i<K;i++)
        {
            for(int j=0;j<(int)N/p;j++)
            {
                local_a[i*(int)N/p+j]=A[i][j*p];
            }
        }
    }
    else
    {
        MPI_Recv(&local_a[0],K*N/p,MPI_INT,master,0,MPI_COMM_WORLD,&status);
    }

    /*for(int i=0;i<(int)K*N/p;i++)
    {
        printf_s("value:%d,rank:%d\n",local_a[i],rank);
    }*/

    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);    
    MPI_File_set_view(fh,sizeof(int)*rank,MPI_INT,filetype,"native",MPI_INFO_NULL);
    MPI_File_read_all(fh,&local_b[0],N/p,MPI_INT,&status);

    for(int i=0;i<(int)N/p;i++)
    {
        printf_s("Value:%d,rank:%d\n",local_b[i],rank);
    }

    for(int i=0;i<K;i++)
    {
        local_d[i]=0;
        for(int j=0;j<N/p;j++)
        {
            local_d[i]+=local_a[i*N/p+j]*local_b[j];
        }
    }
    lmax.rank=rank;
    lmax.value=local_b[0];
    for(int i=0;i<(int)N/p;i++)
    {
        if(local_b[i]>lmax.value)lmax.value=local_b[i];
    }
    MPI_Reduce(&lmax,&gmax,1,MPI_2INT,MPI_MAXLOC,master,MPI_COMM_WORLD);
    MPI_Bcast(&gmax,1,MPI_2INT,master,MPI_COMM_WORLD);
    master=gmax.rank;
    MPI_Reduce(&local_d,&d,K,MPI_INT,MPI_SUM,master,MPI_COMM_WORLD);
    if(rank==master)
    {
        for(int i=0;i<K;i++)
        {
            printf_s("%d ",d[i]);
        }
    }
    MPI_Finalize();
}