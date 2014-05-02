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
    int rc = MPI_SUCCESS;

    MPI_Count c = count/bigmpi_int_max;
    MPI_Count r = count%bigmpi_int_max;

#ifdef BIGMPI_DEBUG
    printf("MPIX_Type_contiguous_x: count = %zu, chunk = %zu, remainder = %zu \n",
            (size_t)count, (size_t)c, (size_t)r );
#endif

    MPI_Datatype chunk;
    rc = MPI_Type_contiguous(bigmpi_int_max, oldtype, &chunk);
    MPI_ASSERT(rc==MPI_SUCCESS);

    MPI_Datatype chunks;
    rc = MPI_Type_contiguous(c, chunk, &chunks);
    MPI_ASSERT(rc==MPI_SUCCESS);

    MPI_Datatype remainder;
    rc = MPI_Type_contiguous(r, oldtype, &remainder);
    MPI_ASSERT(rc==MPI_SUCCESS);

    int typesize;
    rc = MPI_Type_size(oldtype, &typesize);

    MPI_Aint remdisp                   = (MPI_Aint)c*bigmpi_int_max*typesize; /* must explicit-cast to avoid overflow */
    int array_of_blocklengths[2]       = {1,1};
    MPI_Aint array_of_displacements[2] = {0,remdisp};
    MPI_Datatype array_of_types[2]     = {chunks,remainder};
    rc = MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, newtype);
    MPI_ASSERT(rc==MPI_SUCCESS);

    /* For MPIX_Type_contiguous_x to behave like it should, we do not commit the datatype internally. */
    /*
    rc = MPI_Type_commit(newtype);
    MPI_ASSERT(rc==MPI_SUCCESS);
    */

    rc = MPI_Type_free(&chunk);
    MPI_ASSERT(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&chunks);
    MPI_ASSERT(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&remainder);
    MPI_ASSERT(rc==MPI_SUCCESS);

    return MPI_SUCCESS;
}

#if 0
/*
 * Synopsis
 *
 * int MPIX_Type_contiguousv_x(int ncount,
 *                             MPI_Count counts[],
 *                             MPI_Datatype   oldtypes[],
 *                             MPI_Datatype * newtypes[])
 *
 *  Input Parameters
 *
 *   ncount            length of arrays of counts and types
 *   counts            replication count (nonnegative integer)
 *   oldtypes          old datatype (handle array)
 *
 * Output Parameter
 *
 *   newtypes           new datatype (handle array)
 *
 */
int MPIX_Type_contiguousv_x(int ncount, MPI_Count counts[], MPI_Datatype oldtypes[], MPI_Datatype * newtypes[])
{
    int rc = MPI_SUCCESS;

    TODO: Implement this function

    MPI_Count c = count/bigmpi_int_max;
    MPI_Count r = count%bigmpi_int_max;

    MPI_Datatype chunk;
    rc = MPI_Type_contiguous(bigmpi_int_max, oldtype, &chunk);
    MPI_ASSERT(rc==MPI_SUCCESS);

    MPI_Datatype chunks;
    rc = MPI_Type_contiguous(c, chunk, &chunks);
    MPI_ASSERT(rc==MPI_SUCCESS);

    MPI_Datatype remainder;
    rc = MPI_Type_contiguous(r, oldtype, &remainder);
    MPI_ASSERT(rc==MPI_SUCCESS);

    int typesize;
    rc = MPI_Type_size(oldtype, &typesize);

    MPI_Aint remdisp                   = (MPI_Aint)c*bigmpi_int_max*typesize; /* must explicit-cast to avoid overflow */
    int array_of_blocklengths[2]       = {1,1};
    MPI_Aint array_of_displacements[2] = {0,remdisp};
    MPI_Datatype array_of_types[2]     = {chunks,remainder};
    rc = MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, newtype);
    MPI_ASSERT(rc==MPI_SUCCESS);

    /* For MPIX_Type_contiguous_x to behave like it should, we do not commit the datatype internally. */
    /*
    rc = MPI_Type_commit(newtype);
    MPI_ASSERT(rc==MPI_SUCCESS);
    */

    rc = MPI_Type_free(&chunk);
    MPI_ASSERT(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&chunks);
    MPI_ASSERT(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&remainder);
    MPI_ASSERT(rc==MPI_SUCCESS);

    return MPI_SUCCESS;
}
#endif
