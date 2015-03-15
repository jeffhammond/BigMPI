#include "bigmpi_impl.h"

MPI_Datatype MPIX_GCC_FLOAT128;

int MPIX_Init_thread(int * argc, char *** argv, int requested, int * provided)
{
    int rc = MPI_Init_thread(argc, argv, requested, provided);

    MPI_Type_contiguous(sizeof(__float128), MPI_BYTE, &MPIX_GCC_FLOAT128);
    MPI_Type_commit(&MPIX_GCC_FLOAT128);

    return rc;
}

int MPIX_Init(int * argc, char *** argv)
{
    int provided;
    return MPIX_Init_thread(argc, argv, MPI_THREAD_SINGLE, &provided);
}

int MPIX_Finalize(void)
{
    MPI_Type_free(&MPIX_GCC_FLOAT128);
    return MPI_Finalize();    
}


