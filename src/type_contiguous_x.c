#include "bigmpi_impl.h"

/* This function does all the heavy lifting in BigMPI. */

#define MPI_ASSERT(e)  \
        ((void) ((e) ? 0 : MPI_Abort(MPI_COMM_WORLD,1) ))

/*
 * Synopsis
 *
 * int MPIX_Type_contiguous_x(MPI_Count count,
 *                            MPI_Datatype   oldtype,
 *                            MPI_Datatype * newtype)
 *
 *  Input Parameters
 *
 *   count             replication count (nonnegative integer)
 *   oldtype           old datatype (handle)
 *
 * Output Parameter
 *
 *   newtype           new datatype (handle)
 *
 */
int MPIX_Type_contiguous_x(MPI_Count count, MPI_Datatype oldtype, MPI_Datatype * newtype)
{
    MPI_Count c = count/bigmpi_int_max;
    MPI_Count r = count%bigmpi_int_max;

#ifdef BIGMPI_DEBUG
    printf("MPIX_Type_contiguous_x: count = %zu, chunk = %zu, remainder = %zu \n",
            (size_t)count, (size_t)c, (size_t)r );
#endif

    MPI_Datatype chunks;
    MPI_Type_vector(c, bigmpi_int_max, bigmpi_int_max, oldtype, &chunks);

    MPI_Datatype remainder;
    MPI_Type_contiguous(r, oldtype, &remainder);

    /* TODO: Might need MPI_Type_get_extent to be fully general... */
    int typesize;
    MPI_Type_size(oldtype, &typesize);

    MPI_Aint remdisp          = (MPI_Aint)c*bigmpi_int_max*typesize; /* must explicit-cast to avoid overflow */
    int blocklengths[2]       = {1,1};
    MPI_Aint displacements[2] = {0,remdisp};
    MPI_Datatype types[2]     = {chunks,remainder};
    MPI_Type_create_struct(2, blocklengths, displacements, types, newtype);

    MPI_Type_free(&chunks);
    MPI_Type_free(&remainder);

    return MPI_SUCCESS;
}
