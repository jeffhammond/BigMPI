#include <stdio.h>
#include <mpi.h>

#include "verify_buffer.h"

size_t verify_buffer(char *buf, MPI_Count count, int expected_value)
{
    size_t errors = 0;
    for (size_t i = 0; i < (size_t)count; i++) {
        errors += (buf[i] != (unsigned char)expected_value);
    }
    if (errors > 0) {
        printf("There were %zu errors!", errors);
        MPI_Abort(MPI_COMM_WORLD, (int)errors);
    }
    return errors;
}
