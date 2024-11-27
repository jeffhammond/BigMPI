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

    if (size<2) {
        printf("Use 2 or more processes. \n");
        MPI_Finalize();
        return 1;
    }

    int l = (argc > 1) ? atoi(argv[1]) : 2;
    int m = (argc > 2) ? atoi(argv[2]) : 17777;
    MPI_Count n = l * INT_MAX + m;

    char * buf = NULL;

    MPI_Alloc_mem((MPI_Aint)n, MPI_INFO_NULL, &buf);

    memset(buf, rank, (size_t)n);

    size_t errors = 0;
    for (int r = 1; r < size; r++) {

        /* pairwise communication */
        if (rank==r) {
            MPI_Rsend_c(buf, n, MPI_CHAR, 0 /* dst */, r /* tag */, MPI_COMM_WORLD);
        }
        else if (rank==0) {
            MPI_Recv_c(buf, n, MPI_CHAR, r /* src */, r /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            errors += verify_buffer(buf, n, r);
            if (errors > 0) {
                printf("There were %zu errors!", errors);
            }
        }
    }

    MPI_Free_mem(buf);

    if (rank==0 && errors==0) {
        printf("SUCCESS\n");
    }

    MPI_Finalize();

    return 0;
}
