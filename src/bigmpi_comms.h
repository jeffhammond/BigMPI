#ifndef BIGMPI_COMMS_H_INCLUDED
#define BIGMPI_COMMS_H_INCLUDED

extern int bigmpi_comms_defined;

/* This is just a graph communicator that is equivalent to MPI_COMM_WORLD
 * with full connectivity, which is used to implement large-count versions
 * of allgatherv, alltoallv and alltoallw. */
extern MPI_Comm MPI_GRAPH_WORLD;
/* This is just a graph communicator that is equivalent to MPI_COMM_WORLD
 * with connectivity only to rank zero, which is used to implement large-count
 * versions of gatherv and scatterv when the user selects root=0, which we
 * believe is the common case. */
extern MPI_Comm MPI_GRAPH_WORLD_ROOTZERO;

/* These functions are only required to instantiate the aforementioned
 * communicators and are thus not required in programs that do not need
 * these operations. */

int BigMPI_Initialize_comms(void);
int BigMPI_Finalize_comms(void);

#endif // BIGMPI_COMMS_H_INCLUDED
