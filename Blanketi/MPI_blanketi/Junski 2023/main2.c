#include "mpi.h"
#include "stdio.h"

#define K 6
#define putanja "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Junski 2023\\niz.dat"
#define putanja2 "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Junski 2023\\niz2.dat"
int main(int argc,char* argv[])
{
    int rank,p,master=0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Status status;
    int niz[K],niz_citanje[K];
    for(int i=0;i<K;i++)
    {
        niz[i]=rank*K+i;
    }

    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_WRONLY|MPI_MODE_CREATE,MPI_INFO_NULL,&fh);
    MPI_File_seek(fh,K*rank*sizeof(int),MPI_SEEK_SET);
    MPI_File_write_all(fh,&niz[0],K,MPI_INT,&status);
    MPI_File_close(&fh);

    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
    //MPI_File_seek(fh,K*rank*sizeof(int),MPI_SEEK_SET);
    MPI_File_read_at(fh,rank*K*sizeof(int),&niz_citanje[0],K,MPI_INT,&status);
    MPI_File_close(&fh);

    for(int i=0;i<K;i++)
    {
        printf_s("%d ",niz_citanje[i]);
    }
    MPI_Datatype filetype;
    MPI_Type_vector(p,K/p,K,MPI_INT,&filetype);
    MPI_Type_commit(&filetype);
    MPI_File_open(MPI_COMM_WORLD,putanja2,MPI_MODE_WRONLY|MPI_MODE_CREATE,MPI_INFO_NULL,&fh);
    MPI_File_set_view(fh,rank*K/p*sizeof(int),MPI_INT,filetype,"native",MPI_INFO_NULL);
    MPI_File_write_all(fh,&niz_citanje[0],K,MPI_INT,&status);
    MPI_File_close(&fh);

    MPI_Finalize();
}