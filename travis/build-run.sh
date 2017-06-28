#! /bin/sh

# Exit on error
set -ev

TRAVIS_ROOT="$1"
MPI_IMPL="$2"
BUILD_SYSTEM="$3"

# Environment variables
if [ "${TRAVIS_OS_NAME}" = "linux" ] ; then
   export PATH=$TRAVIS_ROOT/mpich/bin:$PATH
   export PATH=$TRAVIS_ROOT/open-mpi/bin:$PATH
fi

# Capture details of build
case "$MPI_IMPL" in
    mpich)
        mpichversion || true
        mpicc -show || true
        ;;
    openmpi)
        # this is missing with Mac build it seems
        ompi_info --arch --config || true
        mpicc --showme:command || true
        # see https://github.com/open-mpi/ompi/issues/2956
        export TMPDIR=/tmp
        ;;
    *)
        echo "Invalid MPI implementation: ${MPI_IMPL}"
        ;;
esac

case "${BUILD_SYSTEM}" in
    autotools)
        ./autogen.sh
        mkdir -p build-autotools
        cd build-autotools
        ../configure CC=mpicc CFLAGS="-std=c99" --disable-static --prefix=/tmp --with-max-int=1048576
        make V=1
        make V=1 install
        make V=1 check
        ;;
    cmake)
        mkdir -p build-cmake
        cd build-cmake
        cmake .. -DCMAKE_C_COMPILER=mpicc -DCMAKE_C_FLAGS="-std=c99" -DBIGMPI_MAX_INT=1048576
        make VERBOSE=1
        make VERBOSE=1 install
        make VERBOSE=1 check
        ;;
    *)
        echo "Invalid build system: ${BUILD_SYSTEM}"
        ;;
esac
