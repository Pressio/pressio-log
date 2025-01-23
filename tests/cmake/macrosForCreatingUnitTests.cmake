
macro(add_utest_serial TESTNAME)
  add_executable(${TESTNAME} ${ARGN} ${GTESTMAINSDIR}/gTestMain_serial.cc)
  target_link_libraries(${TESTNAME} pressio-log gtest_main)
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()
#=====================================================================

macro(add_utest_mpi TESTNAME gMAIN nRANKS)
  set(testNameFinal ${TESTNAME}_np${nRANKS})
  add_executable(${testNameFinal} ${ARGN} ${GTESTMAINSDIR}/${gMAIN}.cc)
  target_link_libraries(${testNameFinal} ${MPI_CXX_LIBRARIES} pressio-log gtest_main)
  add_test(
    NAME ${testNameFinal}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${nRANKS}
    ${MPIEXEC_PREFLAGS} ${testNameFinal} ${MPIEXEC_POSTFLAGS}
    )
endmacro()
#=====================================================================
