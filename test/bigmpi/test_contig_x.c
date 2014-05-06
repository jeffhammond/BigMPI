#include <stdio.h>
#include <mpi.h>
#include "bigmpi.h"

int main(int argc, char* argv[])
{
    const int e = 60;

    MPI_Init(&argc, &argv);

    MPI_Count n = 1;
    int errors = 0;
    for (int i=0; i<e; i++) {
        printf("Building a BigMPI type for count = %zu \n", (size_t)n);

        MPI_Datatype intype = MPI_CHAR, bigtype;
        MPIX_Type_contiguous_x(n, intype, &bigtype);

        MPI_Count nout;
        MPI_Datatype outtype;
        BigMPI_Decode_contiguous_x(bigtype, &nout, &outtype);

        if (n!=nout) {
            printf("in (%zu) and out (%zu) counts do not match!\n", (size_t)n, (size_t)nout);
            errors++;
        }
        if (intype!=outtype) {
            printf("intype (%d) and outtype (%d) do not match!\n", intype, outtype);
            errors++;
        }

        n *= 2;
    }

    MPI_Finalize();

    return errors;
}
