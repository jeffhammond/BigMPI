#include "bigmpi_impl.h"

MPI_Datatype MPIX_GCC_FLOAT128;

/* FIXME with something thread-safe */
static int bigmpi_initialized_mpi;

int MPIX_Init_thread(int * argc, char *** argv, int requested, int * provided)
{
    int is_init;
    MPI_Initialized(&is_init);

    if (!is_init) {
        MPI_Init_thread(argc, argv, requested, provided);
        bigmpi_initialized_mpi = 1;
    } else {
        int thread_level;
        MPI_Query_thread(&thread_level);
        *provided = thread_level;
        bigmpi_initialized_mpi = 0;
    }

    MPI_Type_contiguous(sizeof(__float128), MPI_BYTE, &MPIX_GCC_FLOAT128);
    MPI_Type_commit(&MPIX_GCC_FLOAT128);

    return MPI_SUCCESS;
}

int MPIX_Init(int * argc, char *** argv)
{
    int provided;
    return MPIX_Init_thread(argc, argv, MPI_THREAD_SINGLE, &provided);
}

int MPIX_Finalize(void)
{
    MPI_Type_free(&MPIX_GCC_FLOAT128);

    if (bigmpi_initialized_mpi) {
        MPI_Finalize();
    }

    return MPI_SUCCESS;
}


