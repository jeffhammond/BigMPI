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

/* The displacements vector cannot be represented in the existing set of MPI-3
   functions because it is an integer rather than an MPI_Aint. */

int MPIX_Gatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint adispls[], MPI_Datatype recvtype,
                   int root, MPI_Comm comm)
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

#ifndef BIGMPI_VCOLLS_P2P
#else // BIGMPI_VCOLLS_P2P
    /* There is no easy way to implement large-count using MPI_Gatherv because displs is an int. */

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
                /* TODO: Use nonblocking and waitall here instead. */
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(recvtype, &lb, &extent);
                MPIX_Recv_x(recvbuf+adispls[i]*extent, recvcounts[i], recvtype,
                            i /* source */, i /* tag */, comm, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPIX_Send_x(sendbuf, sendcount, sendtype, root, rank /* tag */, comm);
    }
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}

int MPIX_Scatterv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint adispls[], MPI_Datatype sendtype,
                   void *recvbuf, MPI_Count recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
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

#ifndef BIGMPI_VCOLLS_P2P
#else // BIGMPI_VCOLLS_P2P
    /* There is no easy way to implement large-count using MPI_Gatherv because displs is an int. */

    /* Do the local comm first to avoid deadlock. */
    if (root) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtype, &lb, &extent);
        MPIX_Sendrecv_x(sendbuf, sendcounts[root], sendtype, root, root /* tag */,
                        recvbuf+adispls[root]*extent, recvcount, recvtype, root, root /* tag */,
                        comm, MPI_STATUS_IGNORE);
    }

    /* Do the nonlocal comms... */
    if (root) {
        for (int i=0; i<size; i++) {
            if (i!=root) {
                /* TODO: Use nonblocking and waitall here instead. */
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(sendtype, &lb, &extent);
                MPIX_Send_x(sendbuf+adispls[i]*extent, sendcounts[i], sendtype,
                            i /* source */, i /* tag */, comm);
            }
        }
    } else {
        MPIX_Recv_x(recvbuf, recvcount, recvtype, root, rank /* tag */, comm, MPI_STATUS_IGNORE);
    }
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}

int MPIX_Allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                      void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint adispls[], MPI_Datatype recvtype,
                      MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int size;
    MPI_Comm_size(comm, &size);

#ifndef BIGMPI_VCOLLS_P2P
    void        ** newsendbufs   = malloc(size*sizeof(void*));        assert(newsendbufs!=NULL);
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);

    /* Allgather sends the same data to every process. */
    for (int i=0; i<size; i++) {
        newsendbufs[i] = (void*)sendbuf;
    }

    if (sendcount <= bigmpi_int_max ) {
        for (int i=0; i<size; i++) {
            newsendcounts[i] = sendcount;
            newsendtypes[i]  = sendtype;
        }
    } else {
        for (int i=0; i<size; i++) {
            newsendcounts[i] = 1;
            MPIX_Type_contiguous_x(sendcount, sendtype, &newsendtypes[i]);
            MPI_Type_commit(&newsendtypes[i]);
        }
    }

    MPI_Aint * sdispls = malloc(size*sizeof(MPI_Aint)); assert(sdispls!=NULL);
    MPI_Aint lb /* unused */, extent;
    MPI_Type_get_extent(recvtype, &lb, &extent);
    for (int i=0; i<size; i++) {
        sdispls[i] = (MPI_Aint)i*extent;
    }

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    for (int i=0; i<size; i++) {
        if (recvcounts[i] <= bigmpi_int_max ) {
            newrecvcounts[i] = recvcounts[i];
            newrecvtypes[i]  = recvtype;
        } else {
            newrecvcounts[i] = 1;
            MPIX_Type_contiguous_x(recvcounts[i], recvtype, &newrecvtypes[i]);
            MPI_Type_commit(&newrecvtypes[i]);
        }
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, -1, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(newsendbufs, newsendcounts, sdispls, newsendtypes,
                                recvbuf,     newrecvcounts, adispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    free(newsendbufs);
    free(newsendcounts);
    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
    }
    free(newsendtypes);
    free(sdispls);
    free(newrecvcounts);
    for (int i=0; i<size; i++) {
        MPI_Type_free(&newrecvtypes[i]);
    }
    free(newrecvtypes);
#else // BIGMPI_VCOLLS_P2P
    /* There is no easy way to implement large-count using MPI_Allgatherv because displs is an int. */
    MPI_Request * reqs = malloc(2*size*sizeof(MPI_Request)); assert(reqs!=NULL);
    for (int i=0; i<size; i++) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtypes[i], &lb, &extent);
        MPIX_Irecv_x(recvbuf+rdispls[i]*extent, recvcounts[i], recvtypes[i], i, i /* tag */, comm, &reqs[i]);
        MPIX_Isend_x(sendbuf, sendcount, sendtype, i /* source */, i /* tag */, comm, &reqs[size+i]);
    }
    MPI_Waitall(2*size, reqs, MPI_STATUSES_IGNORE);
    free(reqs);
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}

int MPIX_Alltoallv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], MPI_Datatype sendtype,
                     void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], MPI_Datatype recvtype,
                     MPI_Comm comm)
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

#ifndef BIGMPI_VCOLLS_P2P
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);

    int send_recv_diff = 0;
    for (int i=0; i<size; i++) {
        if (sendcounts[i] <= bigmpi_int_max ) {
            newsendcounts[i] = sendcounts[i];
            newsendtypes[i]  = sendtype;
        } else {
            newsendcounts[i] = 1;
            MPIX_Type_contiguous_x(sendcounts[i], sendtype, &newsendtypes[i]);
            MPI_Type_commit(&newsendtypes[i]);
        }

        /* check if send and recv count+type vectors for identicality and reuse former if yes */
        send_recv_diff += (sendcounts[i]!=recvcounts[i]);
    }
    send_recv_diff += (sendtype!=recvtype);

    int          * newrecvcounts = NULL;
    MPI_Datatype * newrecvtypes  = NULL;

    if (send_recv_diff==0) {
        newrecvcounts = newsendcounts;
        newrecvtypes  = newsendtypes;
    } else {
        newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
        newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
        for (int i=0; i<size; i++) {
            if (recvcounts[i] <= bigmpi_int_max ) {
                newrecvcounts[i] = recvcounts[i];
                newrecvtypes[i]  = recvtype;
            } else {
                newrecvcounts[i] = 1;
                MPIX_Type_contiguous_x(recvcounts[i], recvtype, &newrecvtypes[i]);
                MPI_Type_commit(&newrecvtypes[i]);
            }
        }
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, -1, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf, newsendcounts, sdispls, newsendtypes,
                                recvbuf, newrecvcounts, rdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    free(newsendcounts);
    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
    }
    free(newsendtypes);
    if (send_recv_diff==0) {
        free(newrecvcounts);
        for (int i=0; i<size; i++) {
            MPI_Type_free(&newrecvtypes[i]);
        }
        free(newrecvtypes);
    }
#else // BIGMPI_VCOLLS_P2P
    /* There is no easy way to implement large-count using MPI_Alltoallv because displs is an int. */
    MPI_Request * reqs = malloc(2*size*sizeof(MPI_Request));
    for (int i=0; i<size; i++) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtype, &lb, &extent);
        MPIX_Irecv_x(recvbuf+rdispls[i]*extent, recvcounts[i], recvtype, i, i /* tag */, comm, &reqs[i]);
        MPI_Type_get_extent(sendtype, &lb, &extent);
        MPIX_Isend_x(sendbuf+sdispls[i]*extent, sendcounts[i], sendtype, i /* source */, i /* tag */, comm, &reqs[size+i]);
    }
    MPI_Waitall(2*size, reqs, MPI_STATUSES_IGNORE);
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}

int MPIX_Alltoallw_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                     void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[],
                     MPI_Comm comm)
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

#ifndef BIGMPI_VCOLLS_P2P
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);

    int send_recv_diff = 0;
    for (int i=0; i<size; i++) {
        if (sendcounts[i] <= bigmpi_int_max ) {
            newsendcounts[i] = sendcounts[i];
            newsendtypes[i]  = sendtypes[i];
        } else {
            newsendcounts[i] = 1;
            MPIX_Type_contiguous_x(sendcounts[i], sendtypes[i], &newsendtypes[i]);
            MPI_Type_commit(&newsendtypes[i]);
        }

        /* check if send and recv count+type vectors for identicality and reuse former if yes */
        send_recv_diff += (sendcounts[i]!=recvcounts[i] || sendtypes[i]!=recvtypes[i]);
    }

    int          * newrecvcounts = NULL;
    MPI_Datatype * newrecvtypes  = NULL;

    if (send_recv_diff==0) {
        newrecvcounts = newsendcounts;
        newrecvtypes  = newsendtypes;
    } else {
        newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
        newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
        for (int i=0; i<size; i++) {
            if (recvcounts[i] <= bigmpi_int_max ) {
                newrecvcounts[i] = recvcounts[i];
                newrecvtypes[i]  = recvtypes[i];
            } else {
                newrecvcounts[i] = 1;
                MPIX_Type_contiguous_x(recvcounts[i], recvtypes[i], &newrecvtypes[i]);
                MPI_Type_commit(&newrecvtypes[i]);
            }
        }
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, -1, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf, newsendcounts, sdispls, newsendtypes,
                                recvbuf, newrecvcounts, rdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    free(newsendcounts);
    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
    }
    free(newsendtypes);
    if (send_recv_diff==0) {
        free(newrecvcounts);
        for (int i=0; i<size; i++) {
            MPI_Type_free(&newrecvtypes[i]);
        }
        free(newrecvtypes);
    }
#else // BIGMPI_VCOLLS_P2P
    /* There is no easy way to implement large-count using MPI_Alltoallw because displs is an int. */
    MPI_Request * reqs = malloc(2*size*sizeof(MPI_Request)); assert(reqs!=NULL);
    for (int i=0; i<size; i++) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtypes[i], &lb, &extent);
        MPIX_Irecv_x(recvbuf+rdispls[i]*extent, recvcounts[i], recvtypes[i], i, i /* tag */, comm, &reqs[i]);
        MPI_Type_get_extent(sendtypes[i], &lb, &extent);
        MPIX_Isend_x(sendbuf+sdispls[i]*extent, sendcounts[i], sendtypes[i], i /* source */, i /* tag */, comm, &reqs[size+i]);
    }
    MPI_Waitall(2*size, reqs, MPI_STATUSES_IGNORE);
    free(reqs);
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}
