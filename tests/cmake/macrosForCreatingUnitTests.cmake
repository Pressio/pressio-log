
macro(add_utest_serial TESTNAME)
  add_executable(${TESTNAME} ${ARGN} ${GTESTMAINSDIR}/gTestMain_serial.cc)
  target_link_libraries(${TESTNAME} gtest_main)
  target_include_directories(${TESTNAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()
#=====================================================================

macro(add_utest_mpi TESTNAME gMAIN nRANKS)
  set(testNameFinal ${TESTNAME}_np${nRANKS})
  add_executable(${testNameFinal} ${ARGN} ${GTESTMAINSDIR}/${gMAIN}.cc)
  target_link_libraries(${testNameFinal} ${MPI_CXX_LIBRARIES} gtest_main)
  target_include_directories(${testNameFinal} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  add_test(
    NAME ${testNameFinal}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${nRANKS}
    ${MPIEXEC_PREFLAGS} ${testNameFinal} ${MPIEXEC_POSTFLAGS}
    )
endmacro()
#=====================================================================
