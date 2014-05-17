#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

#include <mpi.h>
#include "bigmpi.h"
#include "verify_buffer.h"

#ifdef BIGMPI_MAX_INT
const MPI_Count test_int_max = BIGMPI_MAX_INT;
#else
#include <limits.h>
const MPI_Count test_int_max = INT_MAX;
#endif

/* Yes, it is technically unsafe to cast MPI_Count to MPI_Aint or size_t without checking,
 * given that MPI_Count might be 128b and MPI_Aint and size_t might be 64b, but BigMPI
 * does not aspire to support communication of more than 8 EiB messages at a time. */

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if ((size*(size+1)/2)>255) {
        printf("Use fewer processes than %d. \n", size);
        MPI_Finalize();
        return 1;
    }

    int l = (argc > 1) ? atoi(argv[1]) : 2;
    int m = (argc > 2) ? atoi(argv[2]) : 17777;
    MPI_Count n = l * test_int_max + m;

    int * baseptr = NULL;
    MPI_Win win;
    /* Allocate all the window memory on rank 0 */
    MPI_Win_allocate((MPI_Aint)(rank==0 ? n : 0), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &baseptr, &win);
    MPI_Win_lock_all(0, win);

    if (rank==0) {
        memset(baseptr, 0, (size_t)n);
        MPI_Win_sync(win);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int * buf = NULL;
    MPI_Alloc_mem((MPI_Aint)n, MPI_INFO_NULL, &buf);
    memset(buf, rank+1, (size_t)n);

    MPIX_Accumulate_x(buf, n/sizeof(int), MPI_INT, 0 /* target */, 0 /* disp */, n/sizeof(int), MPI_INT, MPI_SUM, win);

    if (rank==0) {
        char expected = size*(size+1)/2;
        size_t errors = verify_buffer((char*)buf, n, expected);
        if (errors > 0) {
            printf("There were %zu errors!", errors);
            for (size_t i=0; i<(size_t)n; i++) {
                printf("buf[%zu] = %d (expected %d)\n", i, buf[i], expected);
            }
        }
        if (errors==0) {
            printf("SUCCESS\n");
        }
    }

    MPI_Win_unlock_all(win);
    MPI_Win_free(&win);

    MPI_Finalize();

    return 0;
}
