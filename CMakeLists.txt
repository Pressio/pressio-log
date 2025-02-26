cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
project(pressio-log CXX)

# tests
#=====================================================================
option(PRESSIOLOG_ENABLE_TESTS "Enable testing for pressio-log library" OFF)
if(PRESSIOLOG_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
