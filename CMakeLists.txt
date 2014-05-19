cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

set(PACKAGE_NAME BigMPI)
find_package(MPI)

# generic function to add user-configurable options.
function(add_config_option name help_message default)
  if(NOT DEFINED ${name})
    set(${name} "${default}")
    set(${name} "${default}" CACHE STRING "${help_message}" FORCE)
  endif()

  set(OPTIONS_LIST "${OPTIONS_LIST}\n\n * ${name}=\"${${name}}\",\n   default=\"${default}\"\n   ${help_message}" PARENT_SCOPE)
endfunction(add_config_option)

add_config_option(BIGMPI_MAX_INT "Determines which element count BigMPI will assume to be safe." 1000000)
add_config_option(LIB_LINKAGE_TYPE "Controls which type of library to build. Suggested: SHARED on Linux (creates a shared object \"bigmpi.so\"), STATIC should work for builds on Cray and Windows." "SHARED" false)

if (BIGMPI_MAX_INT)
  add_definitions(-DBIGMPI_MAX_INT=${BIGMPI_MAX_INT})
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_definitions(${MPI_COMPILE_FLAGS})
include_directories(${MPI_INCLUDE_PATH})

add_custom_target(test echo "Tests passed.")

add_subdirectory(src)
add_subdirectory(tests)
