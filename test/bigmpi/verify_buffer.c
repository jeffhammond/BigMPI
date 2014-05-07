#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include <mpi.h>

#include "verify_buffer.h"

/* This is the generic buffer verification for when char is the
 * type and we use memset for assignment. */
size_t verify_buffer(char *buf, MPI_Count count, int expected_value)
{
    assert(count<SIZE_MAX);

    size_t errors = 0;
    for (size_t i = 0; i < (size_t)count; i++) {
        errors += (buf[i] != (unsigned char)expected_value);
    }
    return errors;
}

size_t verify_doubles(double *buf, MPI_Count count, double expected_value)
{
    assert(count<SIZE_MAX);

    size_t errors = 0;
    for (size_t i = 0; i < (size_t)count; i++) {
        errors += ( fabs(buf[i]-expected_value) > 1e-12 );
    }
    return errors;
}
