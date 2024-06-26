#include <mpi.h>
#include <stdio.h>
#include <math.h>

void main(int argc,char* argv[])
{
    int rank, master=0,size;
    
    int tmp=0;

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int sum=rank;
    int sum_send=rank;
    for(int i=0;i<log2(size);i++)
    {
        if((rank/(int)(pow(2,i)))%2==0)
        {
            MPI_Send(&sum_send,1,MPI_INT,rank^(int)(pow(2,i)),i,MPI_COMM_WORLD);
            MPI_Recv(&tmp,1,MPI_INT,rank^(int)(pow(2,i)),i,MPI_COMM_WORLD,&status);

            sum_send+=tmp;
            if(rank>(rank^(int)(pow(2,i))))   
            sum+=tmp;
        }
        else
        {
            MPI_Recv(&tmp,1,MPI_INT,rank^(int)(pow(2,i)),i,MPI_COMM_WORLD,&status); 
            MPI_Send(&sum_send,1,MPI_INT,rank^(int)(pow(2,i)),i,MPI_COMM_WORLD);

            sum_send+=tmp;
            if(rank>(rank^(int)(pow(2,i))))
            sum+=tmp;
        }
    }
    printf_s("Suma u ranku %d je %d",rank,sum);

    MPI_Finalize();
}