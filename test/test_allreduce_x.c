#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <math.h>

#include <mpi.h>
#include "verify_buffer.h"

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size<1) {
        printf("Use 1 or more processes. \n");
        MPI_Finalize();
        return 1;
    }

    int l = (argc > 1) ? atoi(argv[1]) : 2;
    int m = (argc > 2) ? atoi(argv[2]) : 17777;
    MPI_Count n = l * INT_MAX + m;

    long * sbuf = NULL;
    long * rbuf = NULL;

    MPI_Aint bytes = n*sizeof(long);
    MPI_Alloc_mem(bytes, MPI_INFO_NULL, &sbuf);
    MPI_Alloc_mem(bytes, MPI_INFO_NULL, &rbuf);

    for (MPI_Count i=0; i<n; i++) {
        sbuf[i] = rank+1;
    }
    for (MPI_Count i=0; i<n; i++) {
        rbuf[i] = 0;
    }

    /* collective communication */
    MPI_Allreduce_c(sbuf, rbuf, n, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);

    size_t errors = 0;
    long val = (long)size*(size+1)/2;
    errors = verify_longs(rbuf, n, val);
    if (errors) {
        printf("There were %zu errors out of %zu elements!\n", errors, (size_t)n);
        for (MPI_Count i=0; i<n; i++) {
            printf("rbuf[%zu] = %ld (expected %ld - %s)\n",
                    (size_t)i, rbuf[i], val, rbuf[i]==val ? "RIGHT" : "WRONG");
        }
        fflush(stdout);
    }

    MPI_Free_mem(sbuf);
    MPI_Free_mem(rbuf);

    /* TODO: reduce errors across all ranks in case root result is correct
     *       but others are wrong. */
    if (rank==0 && errors==0) {
        printf("SUCCESS\n");
    }

    MPI_Finalize();

    return 0;
}
