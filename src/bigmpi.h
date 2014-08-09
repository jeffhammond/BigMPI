#ifndef BIGMPI_H_INCLUDED
#define BIGMPI_H_INCLUDED

#include <mpi.h>

/* This function does the heavy lifting in BigMPI. */

int MPIX_Type_contiguous_x(MPI_Count count, MPI_Datatype oldtype, MPI_Datatype * newtype);

/* other datatype creation functions */

int MPIX_Type_create_hvector_x(int count,
	MPI_Count array_of_blocklengths[], MPI_Aint array_of_displacements[],
	MPI_Datatype oldtype, MPI_Datatype * newtype);

/* These functions are primarily for internal use but some users may want to use them
 * so they will be in the public API, albeit with a different namespace. */

int BigMPI_Decode_contiguous_x(MPI_Datatype intype, MPI_Count * count, MPI_Datatype * basetype);

int BigMPI_Create_graph_comm(MPI_Comm comm_old, int root, MPI_Comm * comm_dist_graph);

/* This is used in tests to query the compile-time setting. */
MPI_Count BigMPI_Get_max_int(void);

/* All of these functions should just be calling MPIX_Type_contiguous_x and
 * then the associated MPI function with count=1 and the newtype if the count
 * is bigger than INT_MAX and dropping into the regular implementation otherwise. */

/* Point-to-point */

int MPIX_Send_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPIX_Recv_x(void *buf, MPI_Count count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);

int MPIX_Isend_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPIX_Irecv_x(void *buf, MPI_Count count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);

int MPIX_Sendrecv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype, int dest, int sendtag,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int source, int recvtag,
                    MPI_Comm comm, MPI_Status *status);
int MPIX_Sendrecv_replace_x(void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int sendtag,
                            int source, int recvtag, MPI_Comm comm, MPI_Status *status);

int MPIX_Ssend_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPIX_Rsend_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPIX_Issend_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm,
                  MPI_Request *request);
int MPIX_Irsend_x(const void *buf, MPI_Count count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm,
                  MPI_Request *request);

int MPIX_Mrecv_x(void *buf, MPI_Count count, MPI_Datatype datatype, MPI_Message *message, MPI_Status *status);
int MPIX_Imrecv_x(void *buf, MPI_Count count, MPI_Datatype datatype, MPI_Message *message, MPI_Request *request);

/* Collectives */

int MPIX_Bcast_x(void *buffer, MPI_Count count, MPI_Datatype datatype, int root, MPI_Comm comm);
int MPIX_Gather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                  void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPIX_Scatter_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPIX_Allgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPIX_Alltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm);

int MPIX_Ibcast_x(void *buffer, MPI_Count count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Igather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Iscatter_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Iallgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                      void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPIX_Ialltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);

/* Neighborhood collectives */

int MPIX_Neighbor_allgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                              void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPIX_Neighbor_alltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                             void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPIX_Ineighbor_allgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                               void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm,
                               MPI_Request *request);
int MPIX_Ineighbor_alltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                             void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm,
                             MPI_Request *request);

/* Reductions */

int MPIX_Reduce_x(const void *sendbuf, void *recvbuf, MPI_Count count,
                  MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int MPIX_Allreduce_x(const void *sendbuf, void *recvbuf, MPI_Count count,
                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPIX_Reduce_scatter_block_x(const void *sendbuf, void *recvbuf, MPI_Count recvcount,
                                MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPIX_Ireduce_x(const void *sendbuf, void *recvbuf, MPI_Count count,
                   MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Iallreduce_x(const void *sendbuf, void *recvbuf, MPI_Count count,
                      MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPIX_Ireduce_scatter_block_x(const void *sendbuf, void *recvbuf, MPI_Count recvcount,
                                 MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);

/* RMA */

int MPIX_Accumulate_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                      int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                      MPI_Op op, MPI_Win win);
int MPIX_Get_x(void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
               int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype, MPI_Win win);
int MPIX_Put_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
               int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype, MPI_Win win);
int MPIX_Get_accumulate_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                          void *result_addr, MPI_Count result_count, MPI_Datatype result_datatype,
                          int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                          MPI_Op op, MPI_Win win);
int MPIX_Rput_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                MPI_Win win, MPI_Request *request);
int MPIX_Rget_x(void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                MPI_Win win, MPI_Request *request);
int MPIX_Raccumulate_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                       int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                       MPI_Op op, MPI_Win win, MPI_Request *request);
int MPIX_Rget_accumulate_x(const void *origin_addr, MPI_Count origin_count, MPI_Datatype origin_datatype,
                           void *result_addr, MPI_Count result_count, MPI_Datatype result_datatype,
                           int target_rank, MPI_Aint target_disp, MPI_Count target_count, MPI_Datatype target_datatype,
                           MPI_Op op, MPI_Win win, MPI_Request *request);

/* V-collectives */

int MPIX_Gatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[], MPI_Datatype recvtype,
                   int root, MPI_Comm comm);
int MPIX_Scatterv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint displs[], MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype,
                    int root, MPI_Comm comm);
int MPIX_Allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                      void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[], MPI_Datatype recvtype,
                      MPI_Comm comm);
int MPIX_Alltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], MPI_Datatype sendtype,
                     void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], MPI_Datatype recvtype,
                     MPI_Comm comm);
int MPIX_Alltoallw_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                     void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[],
                     MPI_Comm comm);

int MPIX_Neighbor_allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                               void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[],
                               MPI_Datatype recvtype, MPI_Comm comm);
int MPIX_Neighbor_alltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[],
                              MPI_Datatype sendtype, void *recvbuf, const MPI_Count recvcounts[],
                              const MPI_Aint rdispls[], MPI_Datatype recvtype, MPI_Comm comm);
int MPIX_Neighbor_alltoallw_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[],
                              const MPI_Datatype sendtypes[], void *recvbuf, const MPI_Count recvcounts[],
                              const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);

#if 0

/* UNSUPPORTED */

/* These are equivalent to reduce+scatterv and will likely be supported in the future. */

int MPIX_Reduce_scatter_x(const void *sendbuf, void *recvbuf, const MPI_Count recvcounts[],
                          MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPIX_Ireduce_scatter_x(const void *sendbuf, void *recvbuf, const MPI_Count recvcounts[],
                           MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);

/* It is unclear if supporting these is worthwhile... */

int MPIX_Scan_x(const void *sendbuf, void *recvbuf, MPI_Count count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPIX_Exscan_x(const void *sendbuf, void *recvbuf, MPI_Count count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPIX_Iscan_x(const void *sendbuf, void *recvbuf, MPI_Count count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
                 MPI_Request *request);
int MPIX_Iexscan_x(const void *sendbuf, void *recvbuf, MPI_Count count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
                   MPI_Request *request);

/* Nonblocking V-collectives */

/* These are really hard, if not impossible to support, because the argument vectors
 * have to be duplicated for use in Neighborhood_(i)alltoallw, and these vectors cannot
 * be deallocated until the operation has completed, but there is no mechanism to do that. */

int MPIX_Igatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[], MPI_Datatype recvtype,
                    int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Iscatterv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint displs[], MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPIX_Iallgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                       void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[], MPI_Datatype recvtype,
                       MPI_Comm comm, MPI_Request *request);
int MPIX_Ialltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], MPI_Datatype sendtype,
                      void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], MPI_Datatype recvtype,
                      MPI_Comm comm, MPI_Request *request);

int MPIX_Ialltoallw_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                      void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[],
                      MPI_Comm comm, MPI_Request *request);

int MPIX_Ineighbor_allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                                void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[],
                                MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPIX_Ineighbor_alltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[],
                               MPI_Datatype sendtype, void *recvbuf, const MPI_Count recvcounts[],
                               const MPI_Aint rdispls[], MPI_Datatype recvtype, MPI_Comm comm,
int MPIX_Ineighbor_alltoallw_x(const void *sendbuf, const MPI_Count sendcounts[],
                               const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                               void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[],
                               const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);

#endif // UNSUPPORTED

#endif // BIGMPI_H_INCLUDED
