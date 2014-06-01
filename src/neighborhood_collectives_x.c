#include "bigmpi_impl.h"

int MPIX_Neighbor_allgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                              void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    return MPI_SUCCESS;
}

int MPIX_Neighbor_alltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                             void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    return MPI_SUCCESS;
}

int MPIX_Neighbor_allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                               void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint displs[],
                               MPI_Datatype recvtype, MPI_Comm comm)
{
    return MPI_SUCCESS;
}

int MPIX_Neighbor_alltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const int sdispls[],
                              MPI_Datatype sendtype, void *recvbuf, const MPI_Count recvcounts[],
                              const MPI_Aint rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
{
    return MPI_SUCCESS;
}

int MPIX_Neighbor_alltoallw_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[],
                              const MPI_Datatype sendtypes[], void *recvbuf, const MPI_Count recvcounts[],
                              const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm)
{
    return MPI_SUCCESS;
}


