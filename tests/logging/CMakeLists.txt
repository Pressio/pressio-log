add_utest_serial(
  test_logger_serial
  ${CMAKE_CURRENT_SOURCE_DIR}/test_logger_serial.cc)

if (PRESSIO_ENABLE_TPL_MPI)
  add_utest_mpi(
    test_logger_mpi gTestMain_mpi 3
    ${CMAKE_CURRENT_SOURCE_DIR}/test_logger_mpi.cc)
endif()
