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
    int rc = MPI_SUCCESS;

    int is_intercomm;
    MPI_Comm_test_inter(comm, &is_intercomm);
    if (is_intercomm)
        BigMPI_Error("BigMPI does not support intercommunicators yet.\n");

    if (sendbuf==MPI_IN_PLACE)
        BigMPI_Error("BigMPI does not support in-place in the v-collectives.  Sorry. \n");

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);
    MPI_Aint     * newsdispls    = malloc(size*sizeof(MPI_Aint));     assert(newsdispls!=NULL);

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    MPI_Aint     * newrdispls    = malloc(size*sizeof(MPI_Aint));     assert(newrdispls!=NULL);

    BigMPI_Convert_vectors(size,
                           0 /* splat count */, 0, sendcounts,
                           0 /* splat type */, 0, sendtypes,
                           0 /* zero displs */, sdispls,
                           newsendcounts, newsendtypes, newsdispls);

    BigMPI_Convert_vectors(size,
                           0 /* splat count */, 0, recvcounts,
                           0 /* splat type */, 0, recvtypes,
                           0 /* zero displs */, rdispls,
                           newrecvcounts, newrecvtypes, newrdispls);

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, -1, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf, newsendcounts, newsdispls, newsendtypes,
                                recvbuf, newrecvcounts, newrdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
        MPI_Type_free(&newrecvtypes[i]);
    }
    free(newsendcounts);
    free(newsdispls);
    free(newsendtypes);

    free(newrecvcounts);
    free(newrecvtypes);
    free(newrdispls);

    return rc;
}
