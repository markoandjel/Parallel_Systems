#include "mpi.h"
#include "stdio.h"

#define N 4
int main(int argc,char* argv[])
{
    int rank,p,master=0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int q=N/p;
    struct zaposlen{int id;char ime[20];char prezime[20];float plata;};
    //zaposleni[]={{24,"Pera","Peric",45.4},{187,"Mika","Mikic",12.4},{32,"Zika","Zikic",90.43},{35,"Stefan","Cirkovic",400.23}};
    struct zaposlen zaposleni[N];
    struct zaposlen local_zaposleni[q]; 
    struct {float plata;int id;} lmin;
    
    if(rank==master)
    {
        for(int i=0;i<N;i++)
        {
            printf_s("Unesite id zaposlenog\n");
            scanf_s("%d",&zaposleni[i].id);
            printf_s("Unesite ime zaposlenog\n");
            scanf_s("%s",&zaposleni[i].ime);
            printf_s("Unesite prezime zaposlenog\n");
            scanf_s("%s",&zaposleni[i].prezime);
            printf_s("Unesite platu zaposlenog\n");
            scanf_s("%f",&zaposleni[i].plata);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Datatype blok;
    MPI_Datatype types[4]={MPI_INT,MPI_CHAR,MPI_CHAR,MPI_FLOAT};
    int blocklengths[4]={1,20,20,1};
    MPI_Aint base_address;
    MPI_Get_address(&zaposleni,&base_address);
    MPI_Aint displ[4];
    MPI_Get_address(&zaposleni[0].id,&displ[0]);
    MPI_Get_address(&zaposleni[0].ime[0],&displ[1]);
    MPI_Get_address(&zaposleni[0].prezime[0],&displ[2]);
    MPI_Get_address(&zaposleni[0].plata,&displ[3]);
    /*displ[0]=MPI_Aint_diff(displ[0],base_address);
    displ[1]=MPI_Aint_diff(displ[1],base_address);
    displ[2]=MPI_Aint_diff(displ[2],base_address);
    displ[3]=MPI_Aint_diff(displ[3],base_address);*/
    for(int i=0;i<4;i++)
        displ[i]-=base_address;
    MPI_Type_create_struct(4,blocklengths,displ,types,&blok);
    MPI_Type_commit(&blok);
    MPI_Scatter(&zaposleni[0],q,blok,&local_zaposleni[0],q,blok,0,MPI_COMM_WORLD);
    /*for(int i=0;i<q;i++)
    {
        printf_s("%d,%s,%s,%f,%d\n",local_zaposleni[i].id,local_zaposleni[i].ime,local_zaposleni[i].prezime,local_zaposleni[i].plata,rank);
    }*/
    float min=local_zaposleni[0].plata;
    for(int i=0;i<q;i++)
    {
        if(local_zaposleni[i].plata<min)
        {
            min=local_zaposleni[i].plata;
            lmin.id=local_zaposleni[i].id;
        }
    }
    lmin.plata=min;
    float gmin;
    //printf_s("%d\n",lmin.id);
    MPI_Reduce(&min,&gmin,1,MPI_FLOAT,MPI_MIN,master,MPI_COMM_WORLD);
    if(rank==master)
    {
        //printf_s("%d",gmin.id);
        int i=0;
        while(i<N && zaposleni[i].plata!=gmin)
        i++;
        if(i<N)
        {
            printf_s("%d,%f",zaposleni[i].id,zaposleni[i].plata);
        }   
        else
        {
            printf_s("GRESKA");
        }
    }
    MPI_Finalize();
}