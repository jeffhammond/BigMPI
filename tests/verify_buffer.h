#ifndef VERIFY_BUFFER_H
#define VERIFY_BUFFER_H

size_t verify_buffer(char *buf, MPI_Count count, int expected_value);
size_t verify_doubles(double *buf, MPI_Count count, double expected_value);
void set_doubles(double *buf, MPI_Count count, double value);

#endif // VERIFY_BUFFER_H
