cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
project(pressio-log CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)
include(version)

# define header-only library
#=====================================================================
add_library(pressio-log INTERFACE)
target_include_directories(
    pressio-log INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}
)

# install headers
#=====================================================================
include(GNUInstallDirs)
install(
  DIRECTORY include/pressio-log
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# tests
#=====================================================================
option(PRESSIOLOG_ENABLE_TESTS "Enable testing for pressio-log library" OFF)
if(PRESSIOLOG_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
