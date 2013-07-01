#include "bigmpi.h"

int BigMPI_Send(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;
    int newcount = 0;
    if (count<=INT_MAX) {
        newcount = (int)count;
        MPI_Send(buf, newcount, datatype, dest, tag, comm);
    } else {
        MPI_Datatype newtype;
        int remainder;
        BigMPI_Create_type(datatype, count, &newtype, &newcount, &remainder);
        MPI_Send(buf, newcount, datatype, dest, tag, comm);
        MPI_Send(buf, newcount, datatype, dest, tag, comm);
    }
    return
}

int BigMPI_Recv(void *buf, bigmpi_int_t count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{


}
