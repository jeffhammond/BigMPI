#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

#include <mpi.h>
#include "verify_buffer.h"

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int l = (argc > 1) ? atoi(argv[1]) : 2;
    int m = (argc > 2) ? atoi(argv[2]) : 17777;
    MPI_Count n = l * INT_MAX + m;
    MPI_Count c = n/sizeof(double);

    double * baseptr = NULL;
    MPI_Win win;
    /* Allocate all the window memory on rank 0 */
    MPI_Win_allocate((MPI_Aint)(rank==0 ? n : 0), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &baseptr, &win);
    MPI_Win_lock_all(0, win);

    if (rank==0) {
        set_doubles(baseptr, c, 0.0);
        MPI_Win_sync(win);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    double * buf = NULL;
    if ((size==1 && rank==0) || rank==1) {
        MPI_Alloc_mem((MPI_Aint)n, MPI_INFO_NULL, &buf);
        set_doubles(buf, c, 7.0);
        MPI_Put_c(buf, c, MPI_DOUBLE, 0 /* target */, 0 /* disp */, c, MPI_DOUBLE, win);
        set_doubles(buf, c, 17.0);
        MPI_Accumulate_c(buf, c, MPI_DOUBLE, 0 /* target */, 0 /* disp */, c, MPI_DOUBLE, MPI_SUM, win);
        set_doubles(buf, c, 0.0);
        MPI_Get_c(buf, c, MPI_DOUBLE, 0 /* target */, 0 /* disp */, c, MPI_DOUBLE, win);

        double expected = 7.0 + 17.0;
        size_t errors = verify_doubles(buf, c, expected);
        if (errors > 0) {
            printf("There were %zu errors!", errors);
            for (size_t i=0; i<(size_t)c; i++) {
                printf("buf[%zu] = %lf (expected %lf)\n", i, buf[i], expected);
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
