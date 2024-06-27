#include "mpi.h"
#include "stdio.h"
#include "math.h"

#define N 8
#define putanja "D:\\Paralelni sistemi Github\\Parallel_Systems\\Blanketi\\MPI_blanketi\\Oktobarski 2 2023\\okt2-c.dat"

void main(int argc,char*argv[])
{
    int rank,p,master=0,q;
    int A[N][N],b[N];
    MPI_File fh;
    MPI_Status status;
    MPI_Datatype filetype;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    q=(int)sqrt(p);
    int local_a[(N/q)*(N/q)],local_b[N/q];
    MPI_Datatype blok;
    int blocklen[q],disp[q],local_c[N/q],c_pom[N/q];
    int row,col;
    for(int i=0;i<N/q;i++)
    {
        for(int j=0;j<N/q;j++)
        {
            disp[i*N/q+j]=i*q*N+j*q;
            blocklen[i*N/q+j]=1;
        }
    }
    MPI_Type_indexed(q,blocklen,disp,MPI_INT,&blok);
    MPI_Datatype blok_new;
    MPI_Type_create_resized(blok,0,sizeof(int),&blok_new);
    MPI_Type_commit(&blok_new);
    row=rank/q;
    col=rank%q;
    if(rank==master)
    {
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            A[i][j]=i*N+j;
            b[i]=i*2;
        }
    }

    MPI_Comm row_comm;
    MPI_Comm column_comm;
    MPI_Datatype red,red_new;
    MPI_Comm_split(MPI_COMM_WORLD,row,col,&row_comm);
    MPI_Comm_split(MPI_COMM_WORLD,col,row,&column_comm);
    MPI_Type_vector(N/q,1,q,MPI_INT,&red);
    MPI_Type_create_resized(red,0,sizeof(int),&red_new);
    MPI_Type_commit(&red_new);

    int col_rank,row_rank;
    MPI_Comm_rank(row_comm,&row_rank);
    MPI_Comm_rank(column_comm,&col_rank);
    if(col_rank==0)
    {
        MPI_Scatter(&b[0],1,red_new,&local_b[0],N/q,MPI_INT,master,row_comm);
    }
    MPI_Bcast(&local_b[0],N/q,MPI_INT,0,column_comm);

    if(row_rank==0)
    MPI_Bcast(&A[0][0],N*N,MPI_INT,0,column_comm);
    MPI_Scatter(&A[col_rank][0],1,blok_new,&local_a,(N/q)*(N/q),MPI_INT,0,row_comm);
    
    for(int i=0;i<N/q;i++)
    {
        local_c[i]=0;
        for(int j=0;j<N/q;j++)
        {
            local_c[i]+=local_a[i*N/q+j]*local_b[j];
        }
    }
    MPI_Reduce(&local_c[0],&c_pom[0],N,MPI_INT,MPI_SUM,0,row_comm);

    MPI_Type_vector(N/q,1,q,MPI_INT,&filetype);
    MPI_File_open(MPI_COMM_WORLD,putanja,MPI_MODE_WRONLY|MPI_MODE_CREATE,MPI_INFO_NULL,&fh);
    MPI_File_set_view(fh,col_rank*sizeof(int),MPI_INT,filetype,"native",MPI_INFO_NULL);
    if(row_rank==0)
    MPI_File_write(fh,&c_pom[0],N/q,MPI_INT,&status);
     
    MPI_Finalize();
}