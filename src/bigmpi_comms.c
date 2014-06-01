#include "bigmpi_impl.h"

int bigmpi_comms_defined = 0;

MPI_Comm MPI_GRAPH_WORLD;
MPI_Comm MPI_GRAPH_WORLD_ROOTZERO;

/* TODO: These functions are not thread-safe and need a proper mutex. */

int BigMPI_Initialize_comms(void)
{
    if (bigmpi_comms_defined == 1)
        return MPI_SUCCESS;

    BigMPI_Create_graph_comm(MPI_COMM_WORLD, -1, &MPI_GRAPH_WORLD);
    BigMPI_Create_graph_comm(MPI_COMM_WORLD,  0, &MPI_GRAPH_WORLD_ROOTZERO);

    bigmpi_comms_defined = 1;

    return MPI_SUCCESS;
}

int BigMPI_Finalize_comms(void)
{
    if (bigmpi_comms_defined == 1) {
        MPI_Comm_free(&MPI_GRAPH_WORLD);
        MPI_Comm_free(&MPI_GRAPH_WORLD_ROOTZERO);
    }
    bigmpi_comms_defined = 0;

    return MPI_SUCCESS;
}

/*
 * Synopsis
 *
 * int BigMPI_Create_graph_comm(MPI_Comm comm_old, int root, MPI_Comm * graph_comm)
 *
 *  Input Parameter
 *
 *   comm_old           MPI communicator from which to create a graph comm
 *   root               integer id of root.  if -1, create fully connected graph,
 *                      which is appropriate for the all___ collectives.
 *
 * Output Parameters
 *
 *   graph_comm         MPI topology communicator associated with input communicator
 *   rc                 returns the rc from the MPI graph comm create function.
 *
 */
int BigMPI_Create_graph_comm(MPI_Comm comm_old, int root, MPI_Comm * comm_dist_graph)
{
    int rank, size;
    MPI_Comm_rank(comm_old, &rank);
    MPI_Comm_size(comm_old, &size);

    /* in the all case (root == -1), every rank is a destination for every other rank;
     * otherwise, only the root is a destination. */
    int indegree  = (root == -1 || root==rank) ? size : 0;
    /* in the all case (root == -1), every rank is a source for every other rank;
     * otherwise, all non-root processes are the source for only one rank (the root). */
    int outdegree = (root == -1 || root==rank) ? size : 1;

    int * sources      = malloc(indegree*sizeof(int));  assert(sources!=NULL);
    int * destinations = malloc(outdegree*sizeof(int)); assert(destinations!=NULL);

    for (int i=0; i<indegree; i++) {
        sources[i]      = i;
    }
    for (int i=0; i<outdegree; i++) {
        destinations[i] = (root == -1 || root==rank) ? i : root;
    }

    int rc = MPI_Dist_graph_create_adjacent(comm_old,
                indegree,  sources,      indegree==0  ? MPI_WEIGHTS_EMPTY : MPI_UNWEIGHTED,
                outdegree, destinations, outdegree==0 ? MPI_WEIGHTS_EMPTY : MPI_UNWEIGHTED,
                MPI_INFO_NULL, 0 /* reorder */, comm_dist_graph);

    free(sources);
    free(destinations);

    return rc;
}
