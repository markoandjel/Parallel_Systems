#include "mpi.h"
#include "stdio.h"

#define M 5
#define N 4
#define putanja "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Septembarski 2023\\vecB.dat"

int main(int argc,char*argv[])
{
    int rank,p,A[M][N],b[N],C[M],master=0;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int local_a[N/p*M],local_b[N/p],local_c[M];
    struct {int rank;int value;} lmin,gmin;
    if(rank==master)
    {
        for(int i=0;i<M;i++)
        {
            for(int j=0;j<N;j++)
            {
                A[i][j]=i*N+j;
                printf_s("%d ",A[i][j]);
            }
            printf_s("\n");            
        }
        /*for(int i=0;i<N;i++)
           b[i]=i*N;*/

        /*FILE* fp=fopen(putanja,"wb");
        fwrite(b,sizeof(int),N,fp);
        fclose(fp);*/
    }

    MPI_Datatype blok,blok_new;
    MPI_Type_vector(N/p*M,1,p,MPI_INT,&blok);
    MPI_Type_create_resized(blok,0,sizeof(int),&blok_new);
    MPI_Type_commit(&blok_new);
    MPI_Scatter(&A[0][0],1,blok_new,&local_a[0],N/p*M,MPI_INT,0,MPI_COMM_WORLD);

    /*for(int i=0;i<N/p*M;i++)
    {
        printf_s("Value:%d,rank:%d\n",local_a[i],rank);
    }*/

    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
    MPI_Datatype filetype;
    MPI_Type_vector(N/p,1,p,MPI_INT,&filetype);
    MPI_Type_commit(&filetype);
    MPI_File_set_view(fh,rank*sizeof(int),MPI_INT,filetype,"native",MPI_INFO_NULL);
    MPI_File_read_all(fh,&local_b[0],N/p,MPI_INT,&status);

    for(int i=0;i<N/p;i++)
    {
        printf_s("Value:%d,rank:%d\n",local_b[i],rank);
    }
    lmin.value=local_b[0];
    lmin.rank=rank;
    for(int i=0;i<M;i++)
    {
        local_c[i]=0;
        for(int j=0;j<N/p;j++)
        {
            local_c[i]+=local_a[i*N/p+j]*local_b[j];
            if(local_b[j]>lmin.value) lmin.value=local_b[j];
        }
    }
    MPI_Reduce(&lmin,&gmin,1,MPI_2INT,MPI_MINLOC,master,MPI_COMM_WORLD);
    MPI_Bcast(&gmin,1,MPI_2INT,master,MPI_COMM_WORLD);
    master=gmin.rank;

    MPI_Reduce(&local_c[0],&C[0],M,MPI_INT,MPI_SUM,master,MPI_COMM_WORLD);

    if(rank==master)
    {
        printf_s("Rank:%d, C:\n",rank);
        for(int j=0;j<M;j++)
        {
            printf_s("%d ",C[j]);
        }
    }    
    MPI_Finalize();
}