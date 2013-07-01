#ifndef BIGMPI_IMPL_H_INCLUDED
#define BIGMPI_IMPL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#include <mpi.h>

//#define BIGMPI_MAX INT_MAX;
#define BIGMPI_MAX 1000; /* debugging only */
typedef int64_t bigmpi_int_t;

/* BigMPI_Create_type creates the datatype that will hold BIGMPI_MAX of the original datatype
 *                    and returns the relevant arguments for use by the wrappers.
 *
 * oldtype   - original datatype, often a built-in one, must be less than BIGMPI_MAX*BIGMPI_MAX.
 * oldcount  - a presumably large integer.
 * newtype   - a contiguous datatype of BIGMPI_MAX oldtype.
 * newcount  - how many newtype are required to send the message.
 * remainder - count in oldtype of leftover data, must be less than BIGMPI_MAX.
 */
static inline void BigMPI_Create_type(MPI_Datatype   oldtype, bigmpi_int_t   oldcount, 
                                      MPI_Datatype * newtype)
{
    int rc = MPI_SUCCESS;

    bigmpi_int_t c = oldcount/BIGMPI_MAX;
    bigmpi_int_t r = oldcount%BIGMPI_MAX;

    MPI_Datatype chunk;
    rc = MPI_Type_contiguous(BIGMPI_MAX, oldtype, &chunk);
    assert(rc==MPI_SUCCESS);

    MPI_Datatype chunks;
    rc = MPI_Type_contiguous(c, chunk, &chunks);
    assert(rc==MPI_SUCCESS);

    MPI_Datatype remainder;
    rc = MPI_Type_contiguous(r, oldtype, &remainder);
    assert(rc==MPI_SUCCESS);

    int typesize;
    rc = MPI_Type_size(oldtype, &typesize);

    MPI_Aint remdisp                    = c*BIGMPI_MAX*typesize;
    int array_of_blocklengths[2]        = {1,1};
    MPI_Aint array_of_displacements[2]  = {0,remdisp};
    MPI_Datatype array_of_types[2]      = {chunks,remainder};
    rc = MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, newtype);
    assert(rc==MPI_SUCCESS);

    rc = MPI_Type_commit(newtype);
    assert(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&chunk);
    assert(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&chunks);
    assert(rc==MPI_SUCCESS);

    rc = MPI_Type_free(&remainder);
    assert(rc==MPI_SUCCESS);

    return;
}

#endif // BIGMPI_IMPL_H_INCLUDED
