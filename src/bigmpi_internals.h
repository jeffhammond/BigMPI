#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mpi.h>

void BigMPI_Error_impl(const char *file, const int line, const char *func, const char *msg, ...);

#define BigMPI_Error(...) BigMPI_Error_impl(__FILE__,__LINE__,__func__,__VA_ARGS__)
