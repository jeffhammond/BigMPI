#include "bigmpi_impl.h"

int bigmpi_types_defined = 0;

MPI_Datatype MPI_FLOAT_COUNT;
MPI_Datatype MPI_DOUBLE_COUNT;
MPI_Datatype MPI_LONG_COUNT;
MPI_Datatype MPI_SHORT_COUNT;
MPI_Datatype MPI_INT_COUNT;
MPI_Datatype MPI_LONG_DOUBLE_COUNT;

int BigMPI_Initialize_types(void)
{
    MPI_Aint lb /* unused */, extent;
    MPI_Type_get_extent(oldtype, &lb, &extent);

    MPI_Aint remdisp          = (MPI_Aint)c*bigmpi_int_max*extent;
    int blocklengths[2]       = {1,1};
    MPI_Aint displacements[2] = {0,remdisp};
    MPI_Datatype types[2]     = {chunks,remainder};
    MPI_Type_create_struct(2, blocklengths, displacements, types, newtype);

    bigmpi_types_defined = 1;

    return MPI_SUCCESS;
}

int BigMPI_Finalize_types(void)
{

    MPI_Type_free(&chunks);
    MPI_Type_free(&remainder);

    bigmpi_types_defined = 0;

    return MPI_SUCCESS;
}
