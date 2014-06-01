#ifndef BIGMPI_COMMS_H_INCLUDED
#define BIGMPI_COMMS_H_INCLUDED

int BigMPI_Create_graph_comm(MPI_Comm comm_old, int root, MPI_Comm * comm_dist_graph);

#endif // BIGMPI_COMMS_H_INCLUDED
