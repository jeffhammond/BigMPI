#include "bigmpi_impl.h"

int bigmpi_types_defined = 0;

MPI_Datatype MPI_FLOAT_COUNT;
MPI_Datatype MPI_DOUBLE_COUNT;
MPI_Datatype MPI_LONG_COUNT;
MPI_Datatype MPI_SHORT_COUNT;
MPI_Datatype MPI_INT_COUNT;
MPI_Datatype MPI_LONG_DOUBLE_COUNT;

static const MPI_Datatype maxloc_types[6] = { MPI_FLOAT,
                                              MPI_DOUBLE,
                                              MPI_LONG,
                                              MPI_SHORT,
                                              MPI_INT,
                                              MPI_LONG_DOUBLE };

static MPI_Datatype get_pair_type(MPI_Datatype type)
{
    if      (type==MPI_FLOAT)       return MPI_FLOAT_COUNT;
    else if (type==MPI_DOUBLE)      return MPI_DOUBLE_COUNT;
    else if (type==MPI_LONG)        return MPI_LONG_COUNT;
    else if (type==MPI_SHORT)       return MPI_SHORT_COUNT;
    else if (type==MPI_INT)         return MPI_INT_COUNT;
    else if (type==MPI_LONG_DOUBLE) return MPI_LONG_DOUBLE_COUNT;
    else                            return MPI_DATATYPE_NULL;
}

/* TODO: These functions are not thread-safe and need a proper mutex. */

int BigMPI_Initialize_types(void)
{
    if (bigmpi_types_defined == 1)
        return MPI_SUCCESS;

    for (int i=0; i<6; i++) {

        MPI_Datatype itype = maxloc_types[i];
        MPI_Datatype otype = get_pair_type(itype);

        MPI_Aint lb /* unused */, extent;
        MPI_Type_get_extent(itype, &lb, &extent);

        MPI_Aint disp             = extent;
        int blocklengths[2]       = {1,1};
        MPI_Aint displacements[2] = {0,disp};
        MPI_Datatype types[2]     = {itype,MPI_DATATYPE_NULL};
        /* TODO This does not seem like the right way to do this.
         *      Does C have a typeof operator?
         *      At the very least, deduce what MPI_Count is at configure-time. */
        if (sizeof(MPI_Count)==sizeof(long long)) {
            types[1] = MPI_LONG_LONG;
        } else if (sizeof(MPI_Count)==sizeof(long)) {
            types[1] = MPI_LONG;
        } else if (sizeof(MPI_Count)==sizeof(int)) {
            /* This case is pointless because if MPI_Count is only the size of
             * an integer, then there is no need for BigMPI in the first place. */
            types[1] = MPI_INT;
        } else {
            printf("Could not determine the storage size of MPI_Count and thus\n"
                   "cannot create a pair-type associated therewith.\n");
            MPI_Abort(MPI_COMM_WORLD,sizeof(MPI_Count));
        }
        MPI_Type_create_struct(2, blocklengths, displacements, types, &otype);
        MPI_Type_commit(&otype);
    }

    bigmpi_types_defined = 1;

    return MPI_SUCCESS;
}

int BigMPI_Finalize_types(void)
{
    if (bigmpi_types_defined == 1) {
        for (int i=0; i<6; i++) {
            MPI_Datatype itype = maxloc_types[i];
            MPI_Datatype otype = get_pair_type(itype);
            MPI_Type_free(&otype);
        }
    }
    bigmpi_types_defined = 0;

    return MPI_SUCCESS;
}
