#include "bigmpi_impl.h"

int bigmpi_comms_defined = 0;

MPI_Comm MPI_GRAPH_WORLD;
MPI_Comm MPI_GRAPH_WORLD_ROOTZERO;

/* TODO: These functions are not thread-safe and need a proper mutex. */

int BigMPI_Initialize_comms(void)
{
    if (bigmpi_comms_defined == 1)
        return MPI_SUCCESS;

    {
    }

    bigmpi_comms_defined = 1;

    return MPI_SUCCESS;
}

int BigMPI_Finalize_comms(void)
{
    if (bigmpi_comms_defined == 1) {
    }
    bigmpi_comms_defined = 0;

    return MPI_SUCCESS;
}
