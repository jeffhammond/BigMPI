#include "bigmpi_impl.h"

/* The displacements vector cannot be represented in the existing set of MPI-3
   functions because it is an integer rather than an MPI_Aint. */

int MPIX_Gatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], MPI_Datatype recvtype,
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);
    MPI_Aint     * newsdispls    = malloc(size*sizeof(MPI_Aint));     assert(newsdispls!=NULL);

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    MPI_Aint     * newrdispls    = malloc(size*sizeof(MPI_Aint));     assert(newrdispls!=NULL);

    /* Allgather sends the same data to every process. */

    BigMPI_Convert_vectors(size,
                           1 /* splat count */, sendcount, NULL,
                           1 /* splat type */, sendtype, NULL,
                           1 /* zero displs */, NULL,
                           newsendcounts, newsendtypes, newsdispls);

    if (rank==root) {
        BigMPI_Convert_vectors(size,
                               0 /* splat count */, 0, recvcounts,
                               1 /* splat type */, recvtype, NULL,
                               0 /* zero displs */, rdispls,
                               newrecvcounts, newrecvtypes, newrdispls);
    } else {
        BigMPI_Convert_vectors(size,
                               1 /* splat count */, 0, NULL,
                               1 /* splat type */, MPI_DATATYPE_NULL, NULL,
                               1 /* zero displs */, NULL,
                               newrecvcounts, newrecvtypes, newrdispls);
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, root, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf, newsendcounts, newsdispls, newsendtypes,
                                recvbuf,     newrecvcounts, newrdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
        MPI_Type_free(&newrecvtypes[i]);
    }

    free(newsendcounts);
    free(newsendtypes);
    free(newsdispls);

    free(newrecvcounts);
    free(newrecvtypes);
    free(newrdispls);
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Gatherv because displs is an int. */
    MPI_Request * reqs = NULL;
    if (root) {
        reqs = malloc(size*sizeof(MPI_Request)); assert(reqs!=NULL);
        for (int i=0; i<size; i++) {
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(recvtype, &lb, &extent);
                MPIX_Irecv_x(recvbuf+rdispls[i]*extent, recvcounts[i], recvtype,
                             i /* source */, i /* tag */, comm, &reqs[i]);
        }
    }
    MPIX_Send_x(sendbuf, sendcount, sendtype, root, rank /* tag */, comm);
    if (root) {
        MPI_Waitall(size, reqs, MPI_STATUSES_IGNORE);
        free(reqs);
    }
#elif defined(BIGMPI_VCOLLS_RMA)
    BigMPI_Error("TODO: BIGMPI_VCOLLS_RMA\n");
#else // BIGMPI_VCOLLS
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS
    return rc;
}

int MPIX_Scatterv_x(const void *sendbuf, const MPI_Count sendcounts[], const MPI_Aint sdispls[], MPI_Datatype sendtype,
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)

    /* FIXME Displacements stuff ala gatherv... */

    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);
    MPI_Aint     * newsdispls    = malloc(size*sizeof(MPI_Aint));     assert(newsdispls!=NULL);

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    MPI_Aint     * newrdispls    = malloc(size*sizeof(MPI_Aint));     assert(newrdispls!=NULL);

    for (int i=0; i<size; i++) {
        if (rank!=root) {
            newsendcounts[i] = 0;
            newsendtypes[i]  = MPI_DATATYPE_NULL;
            newsdispls[i]    = 0;
        } else {
            newsendcounts[i] = 1;
            MPIX_Type_contiguous_x(sendcounts[i], sendtype, &newsendtypes[i]);
            MPI_Type_commit(&newsendtypes[i]);
            MPI_Aint lb /* unused */, oldextent, newextent;
            MPI_Type_get_extent(sendtype, &lb, &oldextent);
            MPI_Type_get_extent(newsendtypes[i], &lb, &newextent);
            newsdispls[i] = sdispls[i]*oldextent/newextent;
        }
        newrecvcounts[i] = 1;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtypes[i]);
        MPI_Type_commit(&newrecvtypes[i]);
        newrdispls[i] = 0;
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, root, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf, newsendcounts, newsdispls, newsendtypes,
                                recvbuf, newrecvcounts, newrdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
        MPI_Type_free(&newrecvtypes[i]);
    }

    free(newsendcounts);
    free(newsendtypes);
    free(newsdispls);

    free(newrecvcounts);
    free(newrecvtypes);
    free(newrdispls);
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Scatterv because displs is an int. */
    MPI_Request * reqs = NULL;
    if (root) {
        reqs = malloc(size*sizeof(MPI_Request)); assert(reqs!=NULL);
        for (int i=0; i<size; i++) {
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(sendtype, &lb, &extent);
                MPIX_Isend_x(sendbuf+sdispls[i]*extent, sendcounts[i], sendtype,
                             i /* source */, i /* tag */, comm, &reqs[i]);
        }
    }
    MPIX_Recv_x(recvbuf, recvcount, recvtype, root, rank /* tag */, comm, MPI_STATUS_IGNORE);
    if (root) {
        MPI_Waitall(size, reqs, MPI_STATUSES_IGNORE);
        free(reqs);
    }
#elif defined(BIGMPI_VCOLLS_RMA)
    BigMPI_Error("TODO: BIGMPI_VCOLLS_RMA\n");
#else // BIGMPI_VCOLLS
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS
    return rc;
}

int MPIX_Allgatherv_x(const void *sendbuf, MPI_Count sendcount, MPI_Datatype sendtype,
                      void *recvbuf, const MPI_Count recvcounts[], const MPI_Aint rdispls[], MPI_Datatype recvtype,
                      MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int size;
    MPI_Comm_size(comm, &size);

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    /* TODO Copy-and-paste from Gatherv_x implementation once that is debugged.
     *      Just remote the root specialization to get all-case. */
    BigMPI_Error("TODO: BIGMPI_VCOLLS_NBHD_ALLTOALLW\n");
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Allgatherv because displs is an int. */
    MPI_Request * reqs = malloc(2*size*sizeof(MPI_Request)); assert(reqs!=NULL);
    MPI_Aint lb /* unused */, extent;
    MPI_Type_get_extent(recvtype, &lb, &extent);
    for (int i=0; i<size; i++) {
        MPIX_Irecv_x(recvbuf+rdispls[i]*extent, recvcounts[i], recvtype, i, i /* tag */, comm, &reqs[i]);
        MPIX_Isend_x(sendbuf, sendcount, sendtype, i /* source */, i /* tag */, comm, &reqs[size+i]);
    }
    MPI_Waitall(2*size, reqs, MPI_STATUSES_IGNORE);
    free(reqs);
#elif defined(BIGMPI_VCOLLS_RMA)
    BigMPI_Error("TODO: BIGMPI_VCOLLS_RMA\n");
#else // BIGMPI_VCOLLS
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    /* TODO Copy-and-paste from Scatterv_x or Alltoallw_x implementation once that is debugged.
     *      Just remote the root specialization (in the case of Scatterv_x) to get all-case. */
    BigMPI_Error("TODO: BIGMPI_VCOLLS_NBHD_ALLTOALLW\n");
#elif defined(BIGMPI_VCOLLS_P2P)
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
#elif defined(BIGMPI_VCOLLS_RMA)
    BigMPI_Error("TODO: BIGMPI_VCOLLS_RMA\n");
#else // BIGMPI_VCOLLS
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
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
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Alltoallw because displs is an int. */
    MPI_Request * reqs = malloc(2*size*sizeof(MPI_Request)); assert(reqs!=NULL);
    for (int i=0; i<size; i++) {
        /* No extent calculation because alltoallw does not use that. */
        MPIX_Irecv_x(recvbuf+rdispls[i], recvcounts[i], recvtypes[i], i, i /* tag */, comm, &reqs[i]);
        MPIX_Isend_x(sendbuf+sdispls[i], sendcounts[i], sendtypes[i], i /* source */, i /* tag */, comm, &reqs[size+i]);
    }
    MPI_Waitall(2*size, reqs, MPI_STATUSES_IGNORE);
    free(reqs);
#elif defined(BIGMPI_VCOLLS_RMA)
    /* In the RMA implementation, we will treat send as source (buf) and recv as target (win). */
    MPI_Win win;
    /* This is the most (?) conservative approach possible, and assumes that datatypes are
     * noncontiguous and potentially out-of-order. */
    MPI_Aint max_size = 0;
    for (int i=0; i<size; i++) {
        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(recvtypes[i], &lb, &extent);
        MPI_Aint offset = rdispls[i]+recvcounts[i]*extent;
        max_size = (offset > max_size ? offset : max_size);
    }
    MPI_Win_create(recvbuf, max_size, 1, MPI_INFO_NULL, comm, &win);
    MPI_Win_fence(0, win);
    for (int i=0; i<size; i++) {
        MPI_Put(sendbuf+sdispls[i], sendcounts[i], sendtypes[i],
                i, rdispls[i], recvtypes[i], recvtypes[i], win);
    }
    MPI_Win_fence(0, win);
    MPI_Win_free(&win);
#else // BIGMPI_VCOLLS
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS
    return rc;
}
