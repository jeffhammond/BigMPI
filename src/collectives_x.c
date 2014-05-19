#include "bigmpi_impl.h"

int MPIX_Bcast_x(void *buf, MPI_Count count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (count <= bigmpi_int_max )) {
        rc = MPI_Bcast(buf, (int)count, datatype, root, comm);
    } else {
        MPI_Datatype newtype;
        MPIX_Type_contiguous_x(count, datatype, &newtype);
        MPI_Type_commit(&newtype);
        rc = MPI_Bcast(buf, 1, newtype, root, comm);
        MPI_Type_free(&newtype);
    }
    return rc;
}

int MPIX_Gather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                  void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Gather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Gather(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, root, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Gather(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Gather(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Scatter_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Scatter(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Scatter(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, root, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Scatter(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Scatter(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Allgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Allgather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Allgather(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Allgather(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Allgather(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Alltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Alltoall(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Alltoall(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Alltoall(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Alltoall(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Ibcast_x(void *buf, MPI_Count count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request)
{
    int rc = MPI_SUCCESS;

    if (likely (count <= bigmpi_int_max )) {
        rc = MPI_Ibcast(buf, (int)count, datatype, root, comm, request);
    } else {
        MPI_Datatype newtype;
        MPIX_Type_contiguous_x(count, datatype, &newtype);
        MPI_Type_commit(&newtype);
        rc = MPI_Ibcast(buf, 1, newtype, root, comm, request);
        MPI_Type_free(&newtype);
    }
    return rc;
}

int MPIX_Igather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                  void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Igather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm, request);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Igather(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, root, comm, request);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Iscatter_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Iscatter(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm, request);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Iscatter(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, root, comm, request);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Iallgather_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Iallgather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm, request);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Iallgather(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm, request);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Ialltoall_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Ialltoall(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm, request);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Ialltoall(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm, request);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

/* TODO: The displacements vector cannot be represented in the existing set of MPI-3
         functions because it is an integer rather than an MPI_Aint. */

int MPIX_Gatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count * recvcounts, MPI_Aint * adispls, MPI_Datatype recvtype,
                   int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int is_intercomm;
    MPI_Comm_test_inter(comm, &is_intercomm);
    if (is_intercomm) {
        printf("BigMPI does not support intercommunicators yet.\n");
        MPI_Abort(comm,1);
    }

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    /* There is no way to implement large-count using MPI_Gatherv because displs is an int. */

    /* Do the local comm first to avoid deadlock. */
    if (root) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtype, &lb, &extent);
        MPIX_Sendrecv_x(sendbuf, sendcount, sendtype, root, root /* tag */,
                        recvbuf+adispls[root]*extent, recvcounts[root], recvtype, root, root /* tag */,
                        comm, MPI_STATUS_IGNORE);
    }

    /* Do the nonlocal comms... */
    if (root) {
        for (int i=0; i<size; i++) {
            if (i!=root) {
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(recvtype, &lb, &extent);
                MPIX_Recv_x(recvbuf+adispls[i]*extent, recvcounts[i], recvtype,
                            i /* source */, i /* tag */, comm, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPIX_Send_x(sendbuf, sendcount, sendtype, root, rank /* tag */, comm);
    }
    return rc;
}

#if 0
int MPIX_Scatterv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Scatterv(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Scatterv(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, root, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Scatterv(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Scatterv(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, root, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                     void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Allgatherv(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Allgatherv(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Allgatherv(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Allgatherv(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}

int MPIX_Alltoallv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                    void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= bigmpi_int_max && recvcount <= bigmpi_int_max )) {
        rc = MPI_Alltoallv(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
    } else if (sendcount > bigmpi_int_max && recvcount <= bigmpi_int_max ) {
        MPI_Datatype newsendtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPI_Type_commit(&newsendtype);
        rc = MPI_Alltoallv(sendbuf, 1, newsendtype, recvbuf, (int)recvcount, recvtype, comm);
        MPI_Type_free(&newsendtype);
    } else if (sendcount <= bigmpi_int_max && recvcount > bigmpi_int_max ) {
        MPI_Datatype newrecvtype;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Alltoallv(sendbuf, (int)sendcount, sendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newrecvtype);
    } else {
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Alltoallv(sendbuf, 1, newsendtype, recvbuf, 1, newrecvtype, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}
#endif
