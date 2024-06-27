#include "mpi.h"
#include "stdio.h"

#define K 6
#define M 5
#define N 4
#define putanja "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Oktobarski 2023\\matA.dat"

int main(int argc,char*argv[])
{
    int rank,p,A[M][K],B[M][N],C[K][N],master=0;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int local_a[K/p*M],local_c[K/p][N];
    struct {int rank;int value;} lsum,gsum;
    if (rank==master)
    {
        /*for(int i=0;i<M;i++)
        {
            for(int j=0;j<K;j++)
            {  
                A[i][j]=i+j*K;
            }
        }*/
        for(int i=0;i<M;i++)
        {
            for(int j=0;j<N;j++)
            B[i][j]=i*M+j;
        }
        /*printf_s("B:\n");
        for(int i=0;i<M;i++)
        {
            for(int j=0;j<N;j++)
                printf_s("%d ",B[i][j]);
            printf_s("\n");
        }*/
        

        /*FILE* fp=fopen(putanja,"wb");
        fwrite(&A,sizeof(int),M*K,fp);
        fclose(fp);*/
    }
    MPI_Bcast(&B,M*N,MPI_INT,master,MPI_COMM_WORLD);

    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
    MPI_Datatype filetype;
    MPI_Type_vector(M,K/p,K,MPI_INT,&filetype);
    MPI_Type_commit(&filetype);
    
    MPI_File_set_view(fh,rank*K/p*sizeof(int),MPI_INT,filetype,"native",MPI_INFO_NULL);
    MPI_File_read_all(fh,&local_a[0],K/p*M,MPI_INT,&status); 
    MPI_File_close(&fh);

    /*for(int i=0;i<K/p*M;i++)
        printf_s("%d,%d\n",local_a[i],rank);*/

    for(int i=0;i<K/p;i++)
    {
        for(int j=0;j<N;j++)
        {
            local_c[i][j]=0;
            for(int t=0;t<M;t++)
            {
                local_c[i][j]+=local_a[i+t*K/p]*B[t][j];
            }
        }
    }
    lsum.rank=rank;
    lsum.value=0;   
    for(int i=0;i<K/p*M;i++)
    {
        lsum.value+=local_a[i];
    }
    MPI_Reduce(&lsum,&gsum,1,MPI_2INT,MPI_MINLOC,master,MPI_COMM_WORLD);
    MPI_Bcast(&gsum,1,MPI_2INT,master,MPI_COMM_WORLD);
    master=gsum.rank;
    MPI_Gather(&local_c[0],K/p*N,MPI_INT,&C[0][0],K/p*N,MPI_INT,master,MPI_COMM_WORLD);
    if (rank==master)
    {
        printf_s("Rank: %d\n",rank);
        for(int i=0;i<K;i++)
        {
            for(int j=0;j<N;j++)
            {
                printf_s("%d ",C[i][j]);
            }
            printf_s("\n");
        }
    }
    MPI_Finalize();
}