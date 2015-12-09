/* inspired by and derived from https://github.com/nspark/shmemx */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/******************************/
/* mpi.h                      */
typedef int MPI_Datatype;
typedef int MPI_Comm;
typedef size_t MPI_Aint;
typedef uint64_t MPI_Count;
#define MPI_BYTE 134
#define MPI_COMM_WORLD 9999999
#define MPI_SUCCESS 0
/******************************/

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    printf("MPI_Send\n");
    return MPI_SUCCESS;
}

int MPI_Send_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    printf("MPI_Send_x\n");
    return MPI_SUCCESS;
}

#include "../../src/mpi_c11.h"

int main(void)
{
    /* see note above */
    MPI_Send(NULL,(short)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    /* these are all okay */
    MPI_Send(NULL,0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Send(NULL,0L,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Send(NULL,0LL,MPI_BYTE,0,0,MPI_COMM_WORLD);
    /* see note above */
    MPI_Send(NULL,(unsigned)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    /* these are okay, presumably because they match size_t (not sure) */
    MPI_Send(NULL,(unsigned long)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Send(NULL,(unsigned long long)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    /* these are all okay */
    MPI_Send(NULL,(MPI_Aint)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Send(NULL,(MPI_Count)0,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Count c = 0;
    MPI_Aint  a = 0;
    MPI_Send(NULL,a,MPI_BYTE,0,0,MPI_COMM_WORLD);
    MPI_Send(NULL,c,MPI_BYTE,0,0,MPI_COMM_WORLD);
    return 0;
}
