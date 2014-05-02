#include "bigmpi_impl.h"

int MPIX_Bcast_x(void *buf, MPI_Count count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (count <= BIGMPI_MAX_INT )) {
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
        rc = MPI_Gather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
        rc = MPI_Scatter(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, root, comm);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
        rc = MPI_Allgather(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
        rc = MPI_Alltoall(sendbuf, (int)sendcount, sendtype, recvbuf, (int)recvcount, recvtype, comm);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
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

    if (likely (count <= BIGMPI_MAX_INT )) {
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
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

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
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

#if 0

FIXME: need to copy the recvcounts vector with downcasting or otherwise figure this out.

int MPIX_Gatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const MPI_Count *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    if (likely (sendcount <= BIGMPI_MAX_INT && recvcount <= BIGMPI_MAX_INT )) {
        rc = MPI_Gatherv(sendbuf, (int)sendcount, sendtype,
                         recvbuf, newrecvcounts, recvtype,
                         root, comm);
    } else {
        /* We do not specialize for case where only one of the counts is big
         * because datatype construction overhead is trivial compared to moving
         * >2 GiB. */
        MPI_Datatype newsendtype, newrecvtype;
        MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtype);
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtype);
        MPI_Type_commit(&newsendtype);
        MPI_Type_commit(&newrecvtype);
        rc = MPI_Gather(sendbuf, 1, newsendtype,
                        recvbuf, 1, newrecvtype,
                        root, comm);
        MPI_Type_free(&newsendtype);
        MPI_Type_free(&newrecvtype);
    }
    return rc;
}
#endif
