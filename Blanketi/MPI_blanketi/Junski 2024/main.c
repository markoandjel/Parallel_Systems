#include "mpi.h"
#include "stdio.h"

#define N 10
#define fajl "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Junski 2024\\niz.dat"
int main(int argc,char* argv[])
{
    int rank,p,master=0,C;
    float local_sum=0,sum=0,as=0,local_m=0,M=0;
    struct {float value;int rank;} local_max,max;
    float A[N];
    MPI_File fh;
    MPI_Status status;
    MPI_Datatype filetype;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    MPI_Type_contiguous((int)N/p,MPI_FLOAT,&filetype);
    MPI_Type_commit(&filetype);
    float local_a[(int)N/p];
    if (rank==master)
    {
        /*for(int i=0;i<N;i++)
        {
            A[i]=(float)i/2+3.1415;
        }
        
        FILE* fp=fopen("D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Junski 2024\\niz.dat","w");
        fwrite(A,sizeof(float),N,fp);
        fclose(fp);*/
        C=4;
    }
    MPI_Bcast(&C,1,MPI_INT,master,MPI_COMM_WORLD);
    
    MPI_File_open(MPI_COMM_WORLD,fajl,MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
    MPI_File_set_view(fh,sizeof(float)*((int)N/p)*rank,MPI_FLOAT,filetype,"native",MPI_INFO_NULL);
    MPI_File_read_all(fh,&local_a,(int)N/p,MPI_FLOAT,&status);
    for(int i=0;i<(int)N/p;i++)
    {
        local_sum+=local_a[i];
    }
    MPI_Reduce(&local_sum,&sum,1,MPI_FLOAT,MPI_SUM,master,MPI_COMM_WORLD);
    if(rank==master)
    {
        as=sum/N;
        printf_s("AS:%f",as);
    }
    MPI_Bcast(&as,1,MPI_FLOAT,master,MPI_COMM_WORLD);

    local_max.value=local_a[0];
    local_max.rank=rank;
    for(int i=0;i<(int)N/p;i++)
    {
        local_m+=(local_a[i]+as)/C;
        if(local_a[i]>local_max.value) local_max.value=local_a[i];
    }
    MPI_Reduce(&local_max,&max,1,MPI_FLOAT_INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
    MPI_Bcast(&max,1,MPI_FLOAT_INT,master,MPI_COMM_WORLD);
    master=max.rank;

    MPI_Reduce(&local_m,&M,1,MPI_FLOAT,MPI_SUM,master,MPI_COMM_WORLD);
    if(rank==master)
    {
        printf_s("M:%f, rank:%d\n",M,rank);
    }

    MPI_Finalize();
}