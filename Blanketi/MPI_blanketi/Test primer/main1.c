#include <mpi.h>
#include <stdio.h>

#define N 3
#define c 2

void main(int argc,char* argv[])
{
    int rank, master=0,p;
    int sum=0;
    int gsum=0;
    unsigned short flag=0;
    struct {int value;int rank;} lProst,gProst;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    lProst.value=0;
    lProst.rank=rank;

    for(int i=c;i<c+N;i++)
    {
        for(int j=N-1-rank;j>=0;j-=p)
        {
            int tmp=i+j;
            sum+=tmp;
            flag=0;
            for(int k=2;k<tmp/2;k++)
            {
                if(tmp%k==0)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==0)
            {
                lProst.value++;
            }
        }
    }

    printf_s("Lokalna suma u procesu %d je %d",rank,sum);

    MPI_Reduce(&lProst,&gProst,1,MPI_2INT,MPI_MAXLOC,0,MPI_COMM_WORLD);

    MPI_Bcast(&gProst,1,MPI_2INT,0,MPI_COMM_WORLD);

    MPI_Reduce(&sum,&gsum,1,MPI_INT,MPI_SUM,gProst.rank,MPI_COMM_WORLD);

    if(rank==gProst.rank)
    {
        printf_s("\nSuma: %d",gsum);
    }

    MPI_Finalize();
}