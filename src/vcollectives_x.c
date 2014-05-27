#include "bigmpi_impl.h"

/*
 * Synopsis
 *
 * void convert_vectors(..)
 *
 *  Input Parameter
 *
 *  int          num                length of all vectors (unless splat true)
 *  int          splat_old_count    if non-zero, use oldcount instead of iterating over vector (v-to-w)
 *  MPI_Count    oldcount           single count (ignored if splat_old_count==0)
 *  MPI_Count    oldcounts          vector of counts
 *  int          splat_old_type     if non-zero, use oldtype instead of iterating over vector (v-to-w)
 *  MPI_Datatype oldtype            single type (MPI_DATATYPE_NULL if splat_old_type==0)
 *  MPI_Datatype oldtypes           vector of types (NULL if splat_old_type!=0)
 *  int          zero_new_displs    set the displacement to zero (scatter/gather)
 *  MPI_Aint     olddispls          vector of displacements (NULL if zero_new_displs!=0)
 *
 * Output Parameters
 *
 *  int          newcounts
 *  MPI_Datatype newtypes
 *  MPI_Aint     newdispls
 *
 */
#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
static void BigMPI_Convert_vectors(int                num,
                                   int                splat_old_count,
                                   const MPI_Count    oldcount,
                                   const MPI_Count    oldcounts[],
                                   int                splat_old_type,
                                   const MPI_Datatype oldtype,
                                   const MPI_Datatype oldtypes[],
                                   int                zero_new_displs,
                                   const MPI_Aint     olddispls[],
                                         int          newcounts[],
                                         MPI_Datatype newtypes[],
                                         MPI_Aint     newdispls[])
{
    assert(splat_old_count || (oldcounts!=NULL));
    assert(splat_old_type  || (oldtypes!=NULL));
    assert(zero_new_displs || (olddispls!=NULL));

    for (int i=0; i<num; i++) {
        /* counts */
        newcounts[i] = 1;

        /* types */
        MPIX_Type_contiguous_x(oldcounts[i], splat_old_type ? oldtype : oldtypes[i], &newtypes[i]);
        MPI_Type_commit(&newtypes[i]);

        /* displacements */
        MPI_Aint lb /* unused */, oldextent, newextent;
        MPI_Type_get_extent(splat_old_type ? oldtype : oldtypes[i], &lb, &oldextent);
        MPI_Type_get_extent(newtypes[i], &lb, &newextent);
        newdispls[i] = (zero_new_displs ? 0 : olddispls[i]*oldextent/newextent);
    }
    return;
}
#endif

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
    void        ** newsendbufs   = malloc(size*sizeof(void*));        assert(newsendbufs!=NULL);
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);
    MPI_Aint     * newsdispls    = malloc(size*sizeof(MPI_Aint));     assert(newsdispls!=NULL);

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    MPI_Aint     * newrdispls    = malloc(size*sizeof(MPI_Aint));     assert(newrdispls!=NULL);

    /* Allgather sends the same data to every process. */

    /* Consider rolling this info BigMPI_Convert_vectors... */
    for (int i=0; i<size; i++) {
        newsendbufs[i] = (void*) sendbuf;
    }

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
    rc = MPI_Neighbor_alltoallw(newsendbufs, newsendcounts, newsdispls, newsendtypes,
                                recvbuf,     newrecvcounts, newrdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
        MPI_Type_free(&newrecvtypes[i]);
    }

    free(newsendbufs);
    free(newsendcounts);
    free(newsendtypes);
    free(newsdispls);

    free(newrecvcounts);
    free(newrecvtypes);
    free(newrdispls);
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Gatherv because displs is an int. */
    if (root) {
        MPI_Request * reqs = malloc(size*sizeof(MPI_Request)); assert(reqs!=NULL);
        for (int i=0; i<size; i++) {
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(recvtype, &lb, &extent);
                MPIX_Irecv_x(recvbuf+adispls[i]*extent, recvcounts[i], recvtype,
                             i /* source */, i /* tag */, comm, &reqs[i]);
        }
    }
    MPIX_Send_x(sendbuf, sendcount, sendtype, root, rank /* tag */, comm);
    if (root) {
        MPI_Waitall(size, reqs, MPI_STATUSES_IGNORE);
        free(reqs);
    }
#endif // BIGMPI_VCOLLS_P2P
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

    void        ** newrecvbufs   = malloc(size*sizeof(void*));        assert(newrecvbufs!=NULL);
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
        newrecvbufs[i] = (void*)recvbuf;
        newrecvcounts[i] = 1;
        MPIX_Type_contiguous_x(recvcount, recvtype, &newrecvtypes[i]);
        MPI_Type_commit(&newrecvtypes[i]);
        newrdispls[i] = 0;
    }

    MPI_Comm comm_dist_graph;
    BigMPI_Create_graph_comm(comm, root, &comm_dist_graph);
    rc = MPI_Neighbor_alltoallw(sendbuf,     newsendcounts, newsdispls, newsendtypes,
                                newrecvbufs, newrecvcounts, newrdispls, newrecvtypes, comm_dist_graph);
    MPI_Comm_free(&comm_dist_graph);

    for (int i=0; i<size; i++) {
        MPI_Type_free(&newsendtypes[i]);
        MPI_Type_free(&newrecvtypes[i]);
    }

    free(newsendcounts);
    free(newsendtypes);
    free(newsdispls);

    free(newrecvbufs);
    free(newrecvcounts);
    free(newrecvtypes);
    free(newrdispls);
#elif defined(BIGMPI_VCOLLS_P2P)
    /* There is no easy way to implement large-count using MPI_Scatterv because displs is an int. */
    if (root) {
        MPI_Request * reqs = malloc(size*sizeof(MPI_Request)); assert(reqs!=NULL);
        for (int i=0; i<size; i++) {
                MPI_Aint lb /* unused */, extent;
                MPI_Type_get_extent(sendtype, &lb, &extent);
                MPIX_Isend_x(sendbuf+adispls[i]*extent, sendcounts[i], sendtype,
                             i /* source */, i /* tag */, comm, &reqs[i]);
        }
    }
    MPIX_Recv_x(recvbuf, recvcount, recvtype, root, rank /* tag */, comm, MPI_STATUS_IGNORE);
    if (root) {
        MPI_Waitall(size, reqs, MPI_STATUSES_IGNORE);
        free(reqs);
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    /* TODO Copy-and-paste from Gatherv_x implementation once that is debugged.
     *      Just remote the root specialization to get all-case. */
#elif defined(BIGMPI_VCOLLS_P2P)
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    /* TODO Copy-and-paste from Gatherv_x implementation once that is debugged.
     *      Just remote the root specialization to get all-case. */
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

#if defined(BIGMPI_VCOLLS_NBHD_ALLTOALLW)
    int          * newsendcounts = malloc(size*sizeof(int));          assert(newsendcounts!=NULL);
    MPI_Datatype * newsendtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newsendtypes!=NULL);
    MPI_Aint     * newsdispls    = malloc(size*sizeof(MPI_Aint));     assert(newsdispls!=NULL);

    int          * newrecvcounts = malloc(size*sizeof(int));          assert(newrecvcounts!=NULL);
    MPI_Datatype * newrecvtypes  = malloc(size*sizeof(MPI_Datatype)); assert(newrecvtypes!=NULL);
    MPI_Aint     * newrdispls    = malloc(size*sizeof(MPI_Aint));     assert(newrdispls!=NULL);

    for (int i=0; i<size; i++) {
        MPI_Aint lb /* unused */, oldextent, newextent;
        /* We have use a derived type in every case, regardless of the count, because
         * the displacement vector will not be able to hold the offset without them. */
        newsendcounts[i] = 1;
        MPIX_Type_contiguous_x(sendcounts[i], sendtypes[i], &newsendtypes[i]);
        MPI_Type_commit(&newsendtypes[i]);
        MPI_Type_get_extent(sendtypes[i], &lb, &oldextent);
        MPI_Type_get_extent(newsendtypes[i], &lb, &newextent);
        newsdispls[i] = sdispls[i]*oldextent/newextent;

        newrecvcounts[i] = 1;
        MPIX_Type_contiguous_x(recvcounts[i], recvtypes[i], &newrecvtypes[i]);
        MPI_Type_commit(&newrecvtypes[i]);
        MPI_Type_get_extent(recvtypes[i], &lb, &oldextent);
        MPI_Type_get_extent(newrecvtypes[i], &lb, &newextent);
        newrdispls[i] = rdispls[i]*oldextent/newextent;
    }

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
#else
#error NO VCOLLS IMPLEMENTATION CHOSEN!
#endif // BIGMPI_VCOLLS_P2P
    return rc;
}
