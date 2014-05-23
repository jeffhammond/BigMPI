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

/*
 * Synopsis
 *
 * int BigMPI_Create_graph_comm(MPI_Comm old_comm, int root, MPI_Comm * graph_comm)
 *
 *  Input Parameter
 *
 *   old_comm           MPI communicator from which to create a graph comm
 *   root               integer id of root.  if -1, create fully connected graph,
 *                      which is appropriate for the all___ collectives.
 *
 * Output Parameters
 *
 *   graph_comm         MPI topology communicator associated with input communicator
 *   rc                 returns the rc from the MPI graph comm create function.
 *
 */
int BigMPI_Create_graph_comm(MPI_Comm old_comm, int root, MPI_Comm * graph_comm)
{
    int rank, size;
    MPI_Comm_rank(old_comm, &rank);
    MPI_Comm_size(old_comm, &size);





    return MPI_SUCCESS;
}
