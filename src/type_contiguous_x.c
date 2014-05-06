#include "bigmpi_impl.h"

/* This function does all the heavy lifting in BigMPI. */

#define MPI_ASSERT(e)  \
        ((void) ((e) ? 0 : MPI_Abort(MPI_COMM_WORLD,1) ))

/*
 * Synopsis
 *
 * int MPIX_Type_contiguous_x(MPI_Count count,
 *                            MPI_Datatype   oldtype,
 *                            MPI_Datatype * newtype)
 *
 *  Input Parameters
 *
 *   count             replication count (nonnegative integer)
 *   oldtype           old datatype (handle)
 *
 * Output Parameter
 *
 *   newtype           new datatype (handle)
 *
 */
int MPIX_Type_contiguous_x(MPI_Count count, MPI_Datatype oldtype, MPI_Datatype * newtype)
{
    MPI_Count c = count/bigmpi_int_max;
    MPI_Count r = count%bigmpi_int_max;

#ifdef BIGMPI_DEBUG
    printf("MPIX_Type_contiguous_x: count = %zu, chunk = %zu, remainder = %zu \n",
            (size_t)count, (size_t)c, (size_t)r );
#endif

    MPI_Datatype chunks;
    MPI_Type_vector(c, bigmpi_int_max, bigmpi_int_max, oldtype, &chunks);

    MPI_Datatype remainder;
    MPI_Type_contiguous(r, oldtype, &remainder);

    MPI_Aint lb /* unused */, extent;
    MPI_Type_get_extent(oldtype, &lb, &extent);

    MPI_Aint remdisp          = (MPI_Aint)c*bigmpi_int_max*extent;
    int blocklengths[2]       = {1,1};
    MPI_Aint displacements[2] = {0,remdisp};
    MPI_Datatype types[2]     = {chunks,remainder};
    MPI_Type_create_struct(2, blocklengths, displacements, types, newtype);

    MPI_Type_free(&chunks);
    MPI_Type_free(&remainder);

    return MPI_SUCCESS;
}

/*
 * Synopsis
 *
 * This function inverts MPIX_Type_contiguous_x, i.e. it provides
 * the original arguments for that call so that we know how many
 * built-in types are in the user-defined datatype.
 *
 * This function is primary used inside of BigMPI and does not
 * correspond to an MPI function, so we do avoid the use of the
 * MPIX namespace.
 *
 * int BigMPI_Decode_contiguous_x(MPI_Datatype   intype,
 *                                MPI_Count    * count,
 *                                MPI_Datatype * basetype)
 *
 *  Input Parameters
 *
 *   newtype           new datatype (handle)
 *
 * Output Parameter
 *
 *   count             replication count (nonnegative integer)
 *   oldtype           old datatype (handle)
 *
 */
int BigMPI_Decode_contiguous_x(MPI_Datatype intype, MPI_Count * count, MPI_Datatype * basetype)
{
    int nint, nadd, ndts, combiner;

    /* Step 1: Decode the type_create_struct call. */

    MPI_Type_get_envelope(intype, &nint, &nadd, &ndts, &combiner);
    assert(combiner==MPI_COMBINER_STRUCT);
    assert(nint==3);
    assert(nadd==2);
    assert(ndts==2);

    int cnbls[3]; /* {count, blocklengths[]} */
    MPI_Aint displacements[2]; /* {0,remdisp} */
    MPI_Datatype types[2]; /* {chunks,remainder} */;
    MPI_Type_get_contents(intype, 3, 2, 2, cnbls, displacements, types);
    assert(cnbls[0]==2);
    assert(cnbls[1]==1);
    assert(cnbls[2]==1);
    assert(displacements[0]==0);

    /* Step 2: Decode the type_vector call. */

    MPI_Type_get_envelope(types[0], &nint, &nadd, &ndts, &combiner);
    assert(combiner==MPI_COMBINER_VECTOR);
    assert(nint==3);
    assert(nadd==0);
    assert(ndts==1);

    int cbs[3]; /* {count,blocklength,stride} */
    MPI_Datatype vbasetype[1];
    MPI_Type_get_contents(types[0], 3, 0, 1, cbs, NULL, vbasetype);
    assert(/* blocklength = */ cbs[1]==bigmpi_int_max);
    assert(/* stride = */ cbs[2]==bigmpi_int_max);

    /* chunk count - see above */
    MPI_Count c = cbs[0];

    /* Step 3: Decode the type_contiguous call. */

    MPI_Type_get_envelope(types[1], &nint, &nadd, &ndts, &combiner);
    assert(combiner==MPI_COMBINER_CONTIGUOUS);
    assert(nint==1);
    assert(nadd==0);
    assert(ndts==1);

    int ccc[1]; /* {count} */
    MPI_Datatype cbasetype[1];
    MPI_Type_get_contents(types[1], 1, 0, 1, ccc, NULL, cbasetype);

    /* remainder - see above */
    MPI_Count r = ccc[0];

    /* The underlying type of the vector and contig types must match. */
    assert(cbasetype[0]==vbasetype[0]);
    *basetype = cbasetype[0];

    /* This should not overflow because everything is already MPI_Count type. */
    *count = c*bigmpi_int_max+r;

    return MPI_SUCCESS;
}
