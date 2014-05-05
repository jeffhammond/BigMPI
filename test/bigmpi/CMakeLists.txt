file(GLOB TESTS "test_*.c")

foreach(test ${TESTS})
  string(REPLACE ".c" "" buffer0 ${test})
  string(REPLACE "${CMAKE_SOURCE_DIR}" "" buffer1 ${buffer0})
  string(REPLACE "/" _ buffer2 ${buffer1})
  string(REPLACE "_test" "test" executable ${buffer2})

  set(TARGET_UNIT_TEST_EXE ${executable})
  set(TARGET_RUN_UNIT_TEST run_${executable})

  add_executable(${TARGET_UNIT_TEST_EXE} ${test} verify_buffer.c)
  target_link_libraries(${TARGET_UNIT_TEST_EXE} bigmpi ${MPI_LIBRARIES})

  add_custom_target(${TARGET_RUN_UNIT_TEST} echo "running test ${TARGET_UNIT_TEST_EXE}" && bash -c "mpirun -np 2 ${TARGET_UNIT_TEST_EXE}")

  add_dependencies(test ${TARGET_RUN_UNIT_TEST})
  add_dependencies(${TARGET_RUN_UNIT_TEST} ${TARGET_UNIT_TEST_EXE})
endforeach()
