#ifndef BIGMPI_IMPL_H
#define BIGMPI_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <mpi.h>

#include "bigmpi.h"
#include "bigmpi_internals.h"

#include "likely.h"

#ifdef BIGMPI_MAX_INT
static const MPI_Count bigmpi_int_max   = BIGMPI_MAX_INT;
static const MPI_Count bigmpi_count_max = (MPI_Count)BIGMPI_MAX_INT*BIGMPI_MAX_INT;
#else
#include <limits.h>
#include <stdint.h>
static const MPI_Count bigmpi_int_max   = INT_MAX;
/* SIZE_MAX corresponds to size_t, which should be what MPI_Aint is. */
static const MPI_Count bigmpi_count_max = SIZE_MAX;
#endif

#endif // BIGMPI_IMPL_H
