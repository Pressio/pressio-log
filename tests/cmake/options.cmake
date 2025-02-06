# ---------------------------------------------------------------

option(PRESSIO_SILENCE_WARNINGS "Enable or disable warnings" OFF)
if (PRESSIO_SILENCE_WARNINGS)
  add_compile_definitions(PRESSIO_SILENCE_WARNINGS=1)
else ()
  add_compile_definitions(PRESSIO_SILENCE_WARNINGS=0)
endif()

option(PRESSIO_ENABLE_COLORIZED_OUTPUT "Enable or disable colorized logging" OFF)
if (PRESSIO_ENABLE_COLORIZED_OUTPUT)
  add_compile_definitions(PRESSIO_ENABLE_COLORIZED_OUTPUT=1)
else ()
  add_compile_definitions(PRESSIO_ENABLE_COLORIZED_OUTPUT=0)
endif()

## ---------------------------------------------------------------

option(PRESSIO_ENABLE_TPL_MPI "Enable MPI" OFF)

if(PRESSIO_ENABLE_TPL_MPI)
  message(">> Enabling MPI since PRESSIO_ENABLE_TPL_MPI=ON")
  add_compile_definitions(PRESSIO_ENABLE_TPL_MPI=1)
  find_package(MPI REQUIRED)
  link_libraries(${MPI_CXX_LIBRARIES})
  include_directories(SYSTEM ${MPI_CXX_INCLUDE_DIRS})
else()
  add_compile_definitions(PRESSIO_ENABLE_TPL_MPI=0)
endif()
