#ifndef BIGMPI_IMPL_H
#define BIGMPI_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include <mpi.h>

#include "bigmpi.h"

#include "likely.h"

#ifdef BIGMPI_MAX_INT
const MPI_Count bigmpi_int_max = BIGMPI_MAX_INT;
#else
#include <limits.h>
const MPI_Count bigmpi_int_max = INT_MAX;
#endif

#endif // BIGMPI_IMPL_H
