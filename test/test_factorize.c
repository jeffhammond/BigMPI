#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <mpi.h>

/* This function is copied from src/type_contiguous_x.c and must be updated
 * manually if the implementation changes.  It is an internal function so
 * it is static and therefore be cannot call the symbol from the library. */

/*
 * Synopsis
 *
 * int BigMPI_Factorize_count(MPI_Count c, int * a, int *b)
 *
 *  Input Parameter
 *
 *   c                  large count
 *
 * Output Parameters
 *
 *   a, b               integers such that c=a*b and a,b<INT_MAX
 *   rc                 returns 0 if a,b found (success), else 1 (failure)
 *
 */
static int BigMPI_Factorize_count(MPI_Count in, int * a, int *b)
{
    /* Not using bigmpi_int_max because we want to debug the case where the library is actually used. */
    MPI_Count lo = in/INT_MAX+1;
    MPI_Count hi = (MPI_Count)floor(sqrt((double)in));

    for (MPI_Count g=lo; g<hi; g++) {
        MPI_Count rem = in%g;
        if (rem==0) {
            *a = (int)g;
            *b = (int)(in/g);
            return 0;
        }
    }
    *a = -1;
    *b = -1;
    return 1;
}

#define TIMING

int main(int argc, char* argv[])
{
    int rc;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Count max = (argc>1) ? atol(argv[1]) : 1LL<<60;
    MPI_Count inc = size; /* Incremement by nproc to distribute test work. */

    int errors = 0;

#ifdef TIMING
    double t0 = MPI_Wtime();
#endif
    for (MPI_Count count=1; count<max; count+=inc) {
        int a, b;
        rc = BigMPI_Factorize_count(count, &a, &b);
        errors += rc;
#ifndef TIMING
        printf("factorized %zu = %d * %d (rc=%d)\n", (size_t)count, a, b, rc);
#endif
    }
#ifdef TIMING
    double t1 = MPI_Wtime();
    printf("factorize 1 to %zu in %lf s (%lf us per call)\n",
            (size_t)max, t1-t0, 1.e6*(t1-t0)/(double)max);
#endif

    MPI_Finalize();
    return rc;
}
