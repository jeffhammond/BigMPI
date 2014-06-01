#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <mpi.h>
#include "bigmpi.h"

void BigMPI_Error_impl(const char *file, const int line, const char *func, const char *msg, ...);

#define BigMPI_Error(...) BigMPI_Error_impl(__FILE__,__LINE__,__func__,__VA_ARGS__)

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
                                  MPI_Aint     newdispls[]);
