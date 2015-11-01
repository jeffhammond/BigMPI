#! /bin/sh

# Exit on error
set -ev

# Environment variables
export CFLAGS="-std=c99"
#export MPICH_CC=$CC
export MPICC=mpicc

# Configure and build MADNESS
./autogen.sh 
./configure \
    --enable-g \
    --disable-static \
    --with-max-int=1048576 \
    LIBS="-lm -lpthread"

if [ "$RUN_TEST" = "buildonly" ]; then
    # Build all libraries, examples, and applications
    make -j2 all
else
    # Run unit tests
    make check
fi
