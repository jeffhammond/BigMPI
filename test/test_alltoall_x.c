#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

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

    char * buf_send = NULL;
    char * buf_recv = NULL;

    MPI_Alloc_mem((MPI_Aint)n * size, MPI_INFO_NULL, &buf_send);
    assert(buf_send!=NULL);
    MPI_Alloc_mem((MPI_Aint)n * size, MPI_INFO_NULL, &buf_recv);
    assert(buf_recv!=NULL);

    for (int i = 0; i < size; ++i) {
        for (MPI_Count j = 0; j < n; ++j) {
            buf_send[i*n+j] = (unsigned char)i;
        }
    }
    memset(buf_recv, -1, (size_t)n);

    /* collective communication */
    MPI_Alltoall_c(buf_send, n, MPI_CHAR,
                    buf_recv, n, MPI_CHAR,
                    MPI_COMM_WORLD);

    size_t errors = verify_buffer(buf_recv, n, rank);

    MPI_Free_mem(buf_send);
    MPI_Free_mem(buf_recv);

    if (rank==0 && errors==0) {
        printf("SUCCESS\n");
    }

    MPI_Finalize();

    int rc = (errors > 0) ? 1 : 0;
    return rc;
}
