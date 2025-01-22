if(PRESSIO_ENABLE_TPL_MPI)
  message(">> Enabling MPI since PRESSIO_ENABLE_TPL_MPI=ON")
  add_definitions(-DPRESSIO_ENABLE_TPL_MPI)

  find_package(MPI REQUIRED)
  include_directories(SYSTEM ${MPI_CXX_INCLUDE_DIRS})
endif()
