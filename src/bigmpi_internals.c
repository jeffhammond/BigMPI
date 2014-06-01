#include "bigmpi_internals.h"

/* Raise an internal fatal BigMPI error.
 *
 * @param[in] file Current file name (__FILE__)
 * @param[in] line Current line numeber (__LINE__)
 * @param[in] func Current function name (__func__)
 * @param[in] msg  Message to be printed
 * @param[in] code Exit error code
 */
void BigMPI_Error_impl(const char *file, const int line, const char *func, const char *msg, ...)
{
    va_list ap;
    int  disp;
    char string[500];

    disp  = 0;
    va_start(ap, msg);
    disp += vsnprintf(string, 500, msg, ap);
    va_end(ap);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    fprintf(stderr, "[%d] BigMPI Internal error in %s (%s:%d)\n[%d] Message: %s\n",
            rank, func, file, line, rank, string);
    MPI_Abort(MPI_COMM_WORLD, 100);
}

/*
 * Synopsis
 *
 * void convert_vectors(..)
 *
 *  Input Parameter
 *
 *  int          num                length of all vectors (unless splat true)
 *  int          splat_old_count    if non-zero, use oldcount instead of iterating over vector (v-to-w)
 *  MPI_Count    oldcount           single count (ignored if splat_old_count==0)
 *  MPI_Count    oldcounts          vector of counts
 *  int          splat_old_type     if non-zero, use oldtype instead of iterating over vector (v-to-w)
 *  MPI_Datatype oldtype            single type (MPI_DATATYPE_NULL if splat_old_type==0)
 *  MPI_Datatype oldtypes           vector of types (NULL if splat_old_type!=0)
 *  int          zero_new_displs    set the displacement to zero (scatter/gather)
 *  MPI_Aint     olddispls          vector of displacements (NULL if zero_new_displs!=0)
 *
 * Output Parameters
 *
 *  int          newcounts
 *  MPI_Datatype newtypes
 *  MPI_Aint     newdispls
 *
 */
void BigMPI_Convert_vectors(int                num,
                            int                splat_old_count,
                            const MPI_Count    oldcount,
                            const MPI_Count    oldcounts[],
                            int                splat_old_type,
                            const MPI_Datatype oldtype,
                            const MPI_Datatype oldtypes[],
                            int                zero_new_displs,
                            const MPI_Aint     olddispls[],
                                  int          newcounts[],
                                  MPI_Datatype newtypes[],
                                  MPI_Aint     newdispls[])
{
    assert(splat_old_count || (oldcounts!=NULL));
    assert(splat_old_type  || (oldtypes!=NULL));
    assert(zero_new_displs || (olddispls!=NULL));

    for (int i=0; i<num; i++) {
        /* counts */
        newcounts[i] = 1;

        /* types */
        MPIX_Type_contiguous_x(oldcounts[i], splat_old_type ? oldtype : oldtypes[i], &newtypes[i]);
        MPI_Type_commit(&newtypes[i]);

        /* displacements */
        MPI_Aint lb /* unused */, oldextent, newextent;
        MPI_Type_get_extent(splat_old_type ? oldtype : oldtypes[i], &lb, &oldextent);
        MPI_Type_get_extent(newtypes[i], &lb, &newextent);
        newdispls[i] = (zero_new_displs ? 0 : olddispls[i]*oldextent/newextent);
    }
    return;
}
