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
 */
BigMPI_Create_type(MPI_Datatype oldtype, bigmpi_int_t oldcount, MPI_Datatype * newtype);

#endif // BIGMPI_IMPL_H_INCLUDED
