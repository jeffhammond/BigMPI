#include <stdlib.h>
#include <mpi.h>

int foo(void)
{
    short count = 0;
    return MPI_Send(NULL, count, MPI_BYTE, 0, 0, MPI_COMM_SELF);
}
