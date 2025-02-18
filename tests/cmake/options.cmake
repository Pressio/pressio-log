
if (NOT DEFINED PRESSIO_ENABLE_TPL_MPI)
  option(PRESSIO_ENABLE_TPL_MPI "Enable MPI" OFF)
endif ()

if (PRESSIO_ENABLE_TPL_MPI)
  message(">> pressio-log enabling MPI since PRESSIO_ENABLE_TPL_MPI=ON")
  add_compile_definitions(PRESSIO_ENABLE_TPL_MPI=1)
  find_package(MPI REQUIRED)
  link_libraries(${MPI_CXX_LIBRARIES})
  include_directories(SYSTEM ${MPI_CXX_INCLUDE_DIRS})
else ()
  add_compile_definitions(PRESSIO_ENABLE_TPL_MPI=0)
endif ()
