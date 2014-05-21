#ifndef BIGMPI_TYPES_H_INCLUDED
#define BIGMPI_TYPES_H_INCLUDED

/*
 *   The layouts for the types MPI_TYPE_COUNT etc are simply
 *   struct {
 *       type       var;
 *       MPI_Count  loc;
 *   }
 */

extern MPI_Datatype MPI_FLOAT_COUNT;
extern MPI_Datatype MPI_DOUBLE_COUNT;
extern MPI_Datatype MPI_LONG_COUNT;
extern MPI_Datatype MPI_SHORT_COUNT;
extern MPI_Datatype MPI_INT_COUNT;
extern MPI_Datatype MPI_LONG_DOUBLE_COUNT;

/* These functions are only required to instantiate {MAX,MIN}LOC datatypes
 * and is thus not required in programs that do not need these operations. */

int BigMPI_Initialize_types(void);
int BigMPI_Finalize_types(void);

#endif // BIGMPI_TYPES_H_INCLUDED
