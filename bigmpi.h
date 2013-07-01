#ifndef BIGMPI_H_INCLUDED
#define BIGMPI_H_INCLUDED

#include "bigmpi_impl.h"

int BigMPI_Send(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int BigMPI_Recv(void *buf, bigmpi_int_t count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);

/* UNSUPPORTED
 
int BigMPI_Isend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int BigMPI_Irecv(void *buf, bigmpi_int_t count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);

int BigMPI_Sendrecv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, int dest, int sendtag, 
                    void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status); 
int BigMPI_Sendrecv_replace(void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);

int BigMPI_Bsend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int BigMPI_Ssend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int BigMPI_Rsend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int BigMPI_Ibsend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int BigMPI_Issend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int BigMPI_Irsend(const void *buf, bigmpi_int_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);

int BigMPI_Mrecv(void *buf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Message *message, MPI_Status *status);
int BigMPI_Imrecv(void *buf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Message *message, MPI_Request *request); 

int BigMPI_Bcast(void *buffer, bigmpi_int_t count, MPI_Datatype datatype, int root, MPI_Comm comm);
int BigMPI_Gather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                  void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int BigMPI_Gatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                   void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
int BigMPI_Scatter(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                   void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int BigMPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, 
                    void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int BigMPI_Allgather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                     void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int BigMPI_Allgatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                      void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, MPI_Comm comm);
int BigMPI_Alltoall(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                    void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int BigMPI_Alltoallv(const void *sendbuf, const int *sendcounts, const int *sdispls, MPI_Datatype sendtype, 
                     void *recvbuf, const int *recvcounts, const int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);

int BigMPI_Reduce(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int BigMPI_Allreduce(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int BigMPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const bigmpi_int_t recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int BigMPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int BigMPI_Ireduce(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iallreduce(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int BigMPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const bigmpi_int_t recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int BigMPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);

int BigMPI_Ibcast(void *buffer, bigmpi_int_t count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Igather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                   void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Igatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                    void *recvbuf, const bigmpi_int_t recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iscatter(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                    void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iscatterv(const void *sendbuf, const bigmpi_int_t sendcounts[], const int displs[], MPI_Datatype sendtype, 
                     void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iallgather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                      void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iallgatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                       void *recvbuf, const bigmpi_int_t recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int BigMPI_Ialltoall(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype, 
                     void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int BigMPI_Ialltoallv(const void *sendbuf, const bigmpi_int_t sendcounts[], const int sdispls[], MPI_Datatype sendtype, 
                      void *recvbuf, const bigmpi_int_t recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);

int BigMPI_Accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
                      int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win); 
int BigMPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
               int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int BigMPI_Put(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
               int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int BigMPI_Get_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, 
                          int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
int BigMPI_Rput(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
                int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int BigMPI_Rget(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
                int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int BigMPI_Raccumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
                       int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);
int BigMPI_Rget_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, 
                           int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);

int BigMPI_Neighbor_allgather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                              void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm)
int BigMPI_Neighbor_allgatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                               void *recvbuf, const bigmpi_int_t recvcounts[], const int displs[],
                               MPI_Datatype recvtype, MPI_Comm comm)
int BigMPI_Neighbor_alltoall(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                             void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm)
int BigMPI_Neighbor_alltoallv(const void *sendbuf, const bigmpi_int_t sendcounts[], const int sdispls[],
                              MPI_Datatype sendtype, void *recvbuf, const bigmpi_int_t recvcounts[],
                              const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
int BigMPI_Ineighbor_allgather(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                               void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype,
                               MPI_Comm comm, MPI_Request *request);
int BigMPI_Ineighbor_allgatherv(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                                void *recvbuf, const bigmpi_int_t recvcounts[], const int displs[],
                                MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int BigMPI_Ineighbor_alltoall(const void *sendbuf, bigmpi_int_t sendcount, MPI_Datatype sendtype,
                             void *recvbuf, bigmpi_int_t recvcount, MPI_Datatype recvtype, MPI_Comm comm,
                             MPI_Request *request);
int BigMPI_Ineighbor_alltoallv(const void *sendbuf, const bigmpi_int_t sendcounts[], const int sdispls[],
                               MPI_Datatype sendtype, void *recvbuf, const bigmpi_int_t recvcounts[],
                               const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm,
                               MPI_Request *request);

int BigMPI_Scan(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int BigMPI_Exscan(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int BigMPI_Iscan(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int BigMPI_Iexscan(const void *sendbuf, void *recvbuf, bigmpi_int_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);

int BigMPI_Alltoallw(const void *sendbuf, const bigmpi_int_t sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], 
                     void *recvbuf, const bigmpi_int_t recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int BigMPI_Ialltoallw(const void *sendbuf, const bigmpi_int_t sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], 
                      void *recvbuf, const bigmpi_int_t recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
int BigMPI_Neighbor_alltoallw(const void *sendbuf, const bigmpi_int_t sendcounts[], const MPI_Aint sdispls[],
                              const MPI_Datatype sendtypes[], void *recvbuf, const bigmpi_int_t recvcounts[],
                              const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int BigMPI_Ineighbor_alltoallw(const void *sendbuf, const bigmpi_int_t sendcounts[],
                               const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                               void *recvbuf, const bigmpi_int_t recvcounts[], const MPI_Aint rdispls[],
                               const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
*/

#endif // BIGMPI_H_INCLUDED
